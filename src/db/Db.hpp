#ifndef DB_DB_HPP
#define DB_DB_HPP

#include "db/WowClientDB.hpp"
#include "db/rec/AchievementRec.hpp"
#include "db/rec/Cfg_CategoriesRec.hpp"
#include "db/rec/Cfg_ConfigsRec.hpp"
#include "db/rec/ChrClassesRec.hpp"
#include "db/rec/ChrRacesRec.hpp"
#include "db/rec/MapRec.hpp"

extern WowClientDB<AchievementRec> g_achievementDB;
extern WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;
extern WowClientDB<Cfg_ConfigsRec> g_cfg_ConfigsDB;
extern WowClientDB<ChrClassesRec> g_chrClassesDB;
extern WowClientDB<ChrRacesRec> g_chrRacesDB;
extern WowClientDB<MapRec> g_mapDB;

void ClientDBInitialize();

#endif
