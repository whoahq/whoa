#ifndef DB_DB_HPP
#define DB_DB_HPP

#include "db/WowClientDB.hpp"
#include "db/rec/AchievementRec.hpp"
#include "db/rec/AreaTableRec.hpp"
#include "db/rec/Cfg_CategoriesRec.hpp"
#include "db/rec/Cfg_ConfigsRec.hpp"
#include "db/rec/CharBaseInfoRec.hpp"
#include "db/rec/CharHairGeosetsRec.hpp"
#include "db/rec/CharSectionsRec.hpp"
#include "db/rec/CharacterFacialHairStylesRec.hpp"
#include "db/rec/ChrClassesRec.hpp"
#include "db/rec/ChrRacesRec.hpp"
#include "db/rec/CreatureDisplayInfoRec.hpp"
#include "db/rec/CreatureModelDataRec.hpp"
#include "db/rec/FactionGroupRec.hpp"
#include "db/rec/FactionTemplateRec.hpp"
#include "db/rec/ItemDisplayInfoRec.hpp"
#include "db/rec/ItemVisualsRec.hpp"
#include "db/rec/MapRec.hpp"
#include "db/rec/SoundEntriesRec.hpp"
#include "db/rec/SoundEntriesAdvancedRec.hpp"

extern WowClientDB<AchievementRec> g_achievementDB;
extern WowClientDB<AreaTableRec> g_areaTableDB;
extern WowClientDB<Cfg_CategoriesRec> g_cfg_CategoriesDB;
extern WowClientDB<Cfg_ConfigsRec> g_cfg_ConfigsDB;
extern WowClientDB<CharBaseInfoRec> g_charBaseInfoDB;
extern WowClientDB<CharHairGeosetsRec> g_charHairGeosetsDB;
extern WowClientDB<CharSectionsRec> g_charSectionsDB;
extern WowClientDB<CharacterFacialHairStylesRec> g_characterFacialHairStylesDB;
extern WowClientDB<ChrClassesRec> g_chrClassesDB;
extern WowClientDB<ChrRacesRec> g_chrRacesDB;
extern WowClientDB<CreatureDisplayInfoRec> g_creatureDisplayInfoDB;
extern WowClientDB<CreatureModelDataRec> g_creatureModelDataDB;
extern WowClientDB<FactionGroupRec> g_factionGroupDB;
extern WowClientDB<FactionTemplateRec> g_factionTemplateDB;
extern WowClientDB<ItemDisplayInfoRec> g_itemDisplayInfoDB;
extern WowClientDB<ItemVisualsRec> g_itemVisualsDB;
extern WowClientDB<MapRec> g_mapDB;
extern WowClientDB<SoundEntriesRec> g_soundEntriesDB;
extern WowClientDB<SoundEntriesAdvancedRec> g_soundEntriesAdvancedDB;

void ClientDBInitialize();

#endif
