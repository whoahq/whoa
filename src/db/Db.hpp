#ifndef DB_DB_HPP
#define DB_DB_HPP

#include "db/rec/AchievementRec.hpp"
#include "db/WowClientDB.hpp"

extern WowClientDB<AchievementRec> g_achievementDB;

void ClientDBInitialize();

#endif
