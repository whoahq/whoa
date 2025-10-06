#include "db/Db.hpp"
#include "db/WowClientDB_Base.hpp"

WowClientDB<AchievementRec> g_achievementDB;
WowClientDB<AreaTableRec> g_areaTableDB;
WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;
WowClientDB<Cfg_ConfigsRec> g_cfg_ConfigsDB;
WowClientDB<ChrClassesRec> g_chrClassesDB;
WowClientDB<ChrRacesRec> g_chrRacesDB;
WowClientDB<MapRec> g_mapDB;

void LoadDB(WowClientDB_Base* db, const char* filename, int32_t linenumber) {
    db->Load(filename, linenumber);
};

void StaticDBLoadAll(void (*loadFn)(WowClientDB_Base*, const char*, int32_t)) {
    loadFn(&g_achievementDB, __FILE__, __LINE__);
    loadFn(&g_areaTableDB, __FILE__, __LINE__);
    loadFn(&g_cfg_CategoriesDB, __FILE__, __LINE__);
    loadFn(&g_cfg_ConfigsDB, __FILE__, __LINE__);
    loadFn(&g_chrClassesDB, __FILE__, __LINE__);
    loadFn(&g_chrRacesDB, __FILE__, __LINE__);
    loadFn(&g_mapDB, __FILE__, __LINE__);
};

void ClientDBInitialize() {
    // TODO

    StaticDBLoadAll(LoadDB);

    // TODO
}
