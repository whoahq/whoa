/**
 * FETCHFS - Lazy file loading for Emscripten via the Fetch API.
 * Patches FS.open to fetch files on-demand when they don't exist locally.
 */

addToLibrary({
  $FETCHFS__deps: ['$FS'],
  $FETCHFS__postset: 'FETCHFS.staticInit();',
  $FETCHFS: {
    baseUrl: './data/',
    initialized: false,
    originalOpen: null,

    staticInit: function() {
      if (typeof FS !== 'undefined' && FS.open) {
        FETCHFS.originalOpen = FS.open;
        FS.open = FETCHFS.patchedOpen;
        FETCHFS.initialized = true;
      }
    },

    setBaseUrl: function(url) {
      FETCHFS.baseUrl = url;
    },

    // Build the fetch URL for a given path
    buildUrl: function(path) {
      // Remove leading slash and normalize backslashes
      var cleanPath = path.replace(/^\/+/, '').replace(/\\/g, '/');
      return FETCHFS.baseUrl + cleanPath;
    },

    // Ensure parent directories exist in MEMFS
    ensureParentDirs: function(path) {
      var parts = path.split('/').filter(function(p) { return p.length > 0; });
      var current = '';

      for (var i = 0; i < parts.length - 1; i++) {
        current += '/' + parts[i];
        try {
          FS.mkdir(current);
        } catch (e) {
          // Directory might already exist (EEXIST) - that's fine
          if (e.errno !== 20) {
            // Some other error, but we can try to continue
          }
        }
      }
    },

    // Synchronously fetch file contents using XMLHttpRequest
    fetchFileSync: function(path) {
      var url = FETCHFS.buildUrl(path);

      try {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', url, false);  // false = synchronous
        // Use overrideMimeType to get binary data (can't use responseType with sync XHR)
        xhr.overrideMimeType('text/plain; charset=x-user-defined');
        xhr.send(null);

        if (xhr.status === 200 || xhr.status === 0) {  // 0 for file:// protocol
          // Convert the response string to Uint8Array
          var text = xhr.responseText;
          var data = new Uint8Array(text.length);
          for (var i = 0; i < text.length; i++) {
            data[i] = text.charCodeAt(i) & 0xff;
          }
          return data;
        } else {
          return null;
        }
      } catch (err) {
        return null;
      }
    },

    // Normalize a path (resolve . and .., make absolute)
    normalizePath: function(path) {
      // Convert backslashes to forward slashes (Windows-style paths)
      path = path.replace(/\\/g, '/');
      // Make absolute if relative
      if (path[0] !== '/') {
        path = FS.cwd() + '/' + path;
      }
      // Split and resolve . and ..
      var parts = path.split('/');
      var result = [];
      for (var i = 0; i < parts.length; i++) {
        var part = parts[i];
        if (part === '' || part === '.') continue;
        if (part === '..') {
          if (result.length > 0) result.pop();
        } else {
          result.push(part);
        }
      }
      return '/' + result.join('/');
    },

    // Patched open that fetches files on demand
    patchedOpen: function(path, flags, mode) {
      // If path is not a string (e.g., internal FS node), pass through to original
      if (typeof path !== 'string') {
        return FETCHFS.originalOpen(path, flags, mode);
      }

      // Normalize the path upfront so all operations use consistent paths
      var normalizedPath = FETCHFS.normalizePath(path);

      // Try original open first with normalized path
      try {
        var result = FETCHFS.originalOpen(normalizedPath, flags, mode);
        return result;
      } catch (e) {
        // Only handle ENOENT (file/directory not found)
        if (e.errno !== 44) {
          throw e;
        }
      }

      // File not found locally - try to fetch it from server
      var data = FETCHFS.fetchFileSync(normalizedPath);

      if (data) {
        // Ensure parent directories exist
        FETCHFS.ensureParentDirs(normalizedPath);

        // Write the fetched data using FS.writeFile which properly initializes MEMFS nodes
        try {
          FS.writeFile(normalizedPath, data);
        } catch (e) {
          throw new FS.ErrnoError(44);  // ENOENT
        }

        // Now the original open should succeed
        return FETCHFS.originalOpen(normalizedPath, flags, mode);
      }

      // Fetch failed - throw original error
      throw new FS.ErrnoError(44);  // ENOENT
    },
  },

  // C-callable function to set base URL
  fetchfs_set_base_url__deps: ['$FETCHFS'],
  fetchfs_set_base_url: function(url) {
    FETCHFS.setBaseUrl(UTF8ToString(url));
  },
});
