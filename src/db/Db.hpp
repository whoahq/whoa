#ifndef DB_DB_HPP
#define DB_DB_HPP

#include "db/WowClientDB.hpp"
#include "db/rec/AchievementRec.hpp"
#include "db/rec/AreaTableRec.hpp"
#include "db/rec/Cfg_CategoriesRec.hpp"
#include "db/rec/Cfg_ConfigsRec.hpp"
#include "db/rec/CharacterFacialHairStylesRec.hpp"
#include "db/rec/CharHairGeosetsRec.hpp"
#include "db/rec/CharSectionsRec.hpp"
#include "db/rec/ChrClassesRec.hpp"
#include "db/rec/ChrRacesRec.hpp"
#include "db/rec/CreatureDisplayInfoRec.hpp"
#include "db/rec/CreatureModelDataRec.hpp"
#include "db/rec/ItemDisplayInfoRec.hpp"
#include "db/rec/MapRec.hpp"

extern WowClientDB<AchievementRec> g_achievementDB;
extern WowClientDB<AreaTableRec> g_areaTableDB;
extern WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;
extern WowClientDB<Cfg_ConfigsRec> g_cfg_ConfigsDB;
extern WowClientDB<CharacterFacialHairStylesRec> g_characterFacialHairStylesDB;
extern WowClientDB<CharHairGeosetsRec> g_charHairGeosetsDB;
extern WowClientDB<CharSectionsRec> g_charSectionsDB;
extern WowClientDB<ChrClassesRec> g_chrClassesDB;
extern WowClientDB<ChrRacesRec> g_chrRacesDB;
extern WowClientDB<CreatureDisplayInfoRec> g_creatureDisplayInfoDB;
extern WowClientDB<CreatureModelDataRec> g_creatureModelDataDB;
extern WowClientDB<ItemDisplayInfoRec> g_itemDisplayInfoDB;
extern WowClientDB<MapRec> g_mapDB;

void ClientDBInitialize();

#endif
