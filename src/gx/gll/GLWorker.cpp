#include "gx/gll/GLWorker.h"
#include "gx/gll/GLCommand.h"
#include "gx/gll/GLDevice.h"

void* GLWorker::Worker(void* userArg) {
    GLWorker* worker = static_cast<GLWorker*>(userArg);

    GLDevice* device = new GLDevice();
    device->Init(nullptr, "GLWorker", 0x10, GLTF_INVALID);

    auto commandsEnd = worker->m_Commands.end();

    while (!worker->m_UnkA0) {
        while (1) {
            pthread_mutex_lock(&worker->m_Mutex1);

            while (worker->m_Commands.begin() == commandsEnd) {
                pthread_cond_wait(&worker->m_Cond1, &worker->m_Mutex1);
            }

            pthread_mutex_lock(&worker->m_Mutex2);

            if (worker->m_Commands.begin() == commandsEnd) {
                break;
            }

            GLCommand* command = worker->m_Commands.front();
            worker->m_Commands.pop_front();

            pthread_mutex_unlock(&worker->m_Mutex1);

            if (command) {
                command->Execute(device);
                delete command;
            } else {
                pthread_cond_signal(&worker->m_Cond2);
            }

            pthread_mutex_unlock(&worker->m_Mutex2);

            if (worker->m_UnkA0) {
                goto DONE;
            }
        }

        pthread_mutex_unlock(&worker->m_Mutex2);
        pthread_mutex_unlock(&worker->m_Mutex1);
    }

DONE:
    delete device;
    return nullptr;
}

GLWorker::GLWorker(GLDevice* device): m_Commands() {
    this->m_UnkA0 = 0;
    this->m_UnkA1 = 0;
    this->m_Device = device;

    pthread_mutex_init(&this->m_Mutex1, nullptr);
    pthread_cond_init(&this->m_Cond1, nullptr);
    pthread_mutex_init(&this->m_Mutex2, nullptr);
    pthread_cond_init(&this->m_Cond2, nullptr);

    pthread_create(&this->m_Thread, nullptr, &Worker, this);
}

void GLWorker::Lock() {
    pthread_mutex_lock(&this->m_Mutex1);
}

void GLWorker::Send(GLCommand* command) {
    this->m_Commands.push_back(command);
    this->m_UnkA1 = 0;
}

void GLWorker::Signal() {
    pthread_cond_signal(&this->m_Cond1);
}

void GLWorker::Unlock() {
    pthread_mutex_unlock(&this->m_Mutex1);
}

void GLWorker::WaitOnGLObjects() {
    pthread_mutex_lock(&this->m_Mutex1);

    if (this->m_Commands.begin() == this->m_Commands.end()) {
        GLFlush* command = new GLFlush();
        this->m_Commands.push_back(command);

        this->m_Commands.push_back(nullptr);
    } else {
        auto it = this->m_Commands.begin();

        // TODO some kind of reordering logic for commands

        if (this->m_Commands.begin() == it) {
            GLFlush* command = new GLFlush();
            this->m_Commands.push_back(command);

            this->m_Commands.push_back(nullptr);
        }  else {
            GLFlush* command = new GLFlush();
            this->m_Commands.insert(it, command);

            this->m_Commands.insert(it, nullptr);
        }
    }

    pthread_cond_signal(&this->m_Cond1);
    pthread_cond_wait(&this->m_Cond2, &this->m_Mutex1);

    this->m_UnkA1 = 1;

    pthread_mutex_unlock(&this->m_Mutex1);
}
