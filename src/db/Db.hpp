#ifndef DB_DB_HPP
#define DB_DB_HPP

#include "db/rec/AchievementRec.hpp"
#include "db/rec/Cfg_CategoriesRec.hpp"
#include "db/WowClientDB.hpp"

extern WowClientDB<AchievementRec> g_achievementDB;
extern WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;

void ClientDBInitialize();

#endif
