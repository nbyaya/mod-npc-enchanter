/*

# Enchanter NPC #

_This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))_

### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: NPC (ID: 601015)
- Script: npc_enchantment
- Config: Yes
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2019.04.15 - Ported to AC by gtao725 (https://github.com/gtao725/)
- v2019.02.21 - Add AI/Phrases/Emotes, Update Menu
- v2018.12.05 - Fix broken menu. Replace 'Enchant Weapon' function. Add creature AI and creature text.
- v2018.12.01 - Update function, Add icons, Fix typos, Add a little personality (Emotes don't always work)
- v2017.08.08 - Release


### CREDITS
------------------------------------------------------------------------------------------------------------------
![Styx](https://stygianthebest.github.io/assets/img/avatar/avatar-128.jpg "Styx")
![StygianCore](https://stygianthebest.github.io/assets/img/projects/stygiancore/StygianCore.png "StygianCore")

##### This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))

#### Additional Credits

- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [OwnedCore](http://ownedcore.com/)
- [ModCraft.io](http://modcraft.io/)
- [MMO Society](https://www.mmo-society.com/)
- [AoWoW](https://wotlk.evowow.com/)
- [More credits are cited in the sources](https://github.com/StygianTheBest)

### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"

enum Enchants
{
    ENCHANT_WEP_BERSERKING = 3789,
    ENCHANT_WEP_BLADE_WARD = 3869,
    ENCHANT_WEP_BLOOD_DRAINING = 3870,
    ENCHANT_WEP_ACCURACY = 3788,
    ENCHANT_WEP_AGILITY_1H = 1103,
    ENCHANT_WEP_SPIRIT = 3844,
    ENCHANT_WEP_BATTLEMASTER = 2675,
    ENCHANT_WEP_BLACK_MAGIC = 3790,
    ENCHANT_WEP_ICEBREAKER = 3239,
    ENCHANT_WEP_LIFEWARD = 3241,
    ENCHANT_WEP_MIGHTY_SPELL_POWER = 3834, // One-hand
    ENCHANT_WEP_EXECUTIONER = 3225,
    ENCHANT_WEP_POTENCY = 3833,
    ENCHANT_WEP_TITANGUARD = 3851,
    ENCHANT_2WEP_MASSACRE = 3827,
    ENCHANT_2WEP_SCOURGEBANE = 3247,
    ENCHANT_2WEP_GIANT_SLAYER = 3251,
    ENCHANT_2WEP_GREATER_SPELL_POWER = 3854,
    ENCHANT_2WEP_AGILITY = 2670,
    ENCHANT_2WEP_MONGOOSE = 2673,

    ENCHANT_SHIELD_DEFENSE = 1952,
    ENCHANT_SHIELD_INTELLECT = 1128,
    ENCHANT_SHIELD_RESILIENCE = 3229,
    ENCHANT_SHIELD_BLOCK = 2655,
    ENCHANT_SHIELD_STAMINA = 1071,
    ENCHANT_SHIELD_TOUGHSHIELD = 2653,
    ENCHANT_SHIELD_TITANIUM_PLATING = 3849,

    ENCHANT_HEAD_BLISSFUL_MENDING = 3819,
    ENCHANT_HEAD_BURNING_MYSTERIES = 3820,
    ENCHANT_HEAD_DOMINANCE = 3796,
    ENCHANT_HEAD_SAVAGE_GLADIATOR = 3842,
    ENCHANT_HEAD_STALWART_PROTECTOR = 3818,
    ENCHANT_HEAD_TORMENT = 3817,
    ENCHANT_HEAD_TRIUMPH = 3795,
    ENCHANT_HEAD_ECLIPSED_MOON = 3815,
    ENCHANT_HEAD_FLAME_SOUL = 3816,
    ENCHANT_HEAD_FLEEING_SHADOW = 3814,
    ENCHANT_HEAD_FROSTY_SOUL = 3812,
    ENCHANT_HEAD_TOXIC_WARDING = 3813,

    ENCHANT_SHOULDER_MASTERS_AXE = 3835,
    ENCHANT_SHOULDER_MASTERS_CRAG = 3836,
    ENCHANT_SHOULDER_MASTERS_PINNACLE = 3837,
    ENCHANT_SHOULDER_MASTERS_STORM = 3838,
    ENCHANT_SHOULDER_GREATER_AXE = 3808,
    ENCHANT_SHOULDER_GREATER_CRAG = 3809,
    ENCHANT_SHOULDER_GREATER_GLADIATOR = 3852,
    ENCHANT_SHOULDER_GREATER_PINNACLE = 3811,
    ENCHANT_SHOULDER_GREATER_STORM = 3810,
    ENCHANT_SHOULDER_DOMINANCE = 3794,
    ENCHANT_SHOULDER_TRIUMPH = 3793,

    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728,
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859,
    ENCHANT_CLOAK_WISDOM = 3296,
    ENCHANT_CLOAK_TITANWEAVE = 1951,
    ENCHANT_CLOAK_SPELL_PIERCING = 3243,
    ENCHANT_CLOAK_SHADOW_ARMOR = 3256,
    ENCHANT_CLOAK_MIGHTY_ARMOR = 3294,
    ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
    ENCHANT_CLOAK_GREATER_SPEED = 3831,

    ENCHANT_LEG_EARTHEN = 3853,
    ENCHANT_LEG_FROSTHIDE = 3822,
    ENCHANT_LEG_ICESCALE = 3823,
    ENCHANT_LEG_BRILLIANT_SPELLTHREAD = 3719,
    ENCHANT_LEG_SAPPHIRE_SPELLTHREAD = 3721,
    ENCHANT_LEG_DRAGONSCALE = 3331,
    ENCHANT_LEG_WYRMSCALE = 3332,

    ENCHANT_GLOVES_GREATER_BLASTING = 3249,
    ENCHANT_GLOVES_ARMSMAN = 3253,
    ENCHANT_GLOVES_CRUSHER = 1603,
    ENCHANT_GLOVES_AGILITY = 3222,
    ENCHANT_GLOVES_PRECISION = 3234,
    ENCHANT_GLOVES_EXPERTISE = 3231,
    ENCHANT_GLOVES_HYPERSPEED = 3604,

    ENCHANT_BRACERS_MAJOR_STAMINA = 3850,
    ENCHANT_BRACERS_SUPERIOR_SP = 2332,
    ENCHANT_BRACERS_GREATER_ASSUALT = 3845,
    ENCHANT_BRACERS_MAJOR_SPIRT = 1147,
    ENCHANT_BRACERS_EXPERTISE = 3231,
    ENCHANT_BRACERS_GREATER_STATS = 2661,
    ENCHANT_BRACERS_INTELLECT = 1119,
    ENCHANT_BRACERS_FURL_ARCANE = 3763,
    ENCHANT_BRACERS_FURL_FIRE = 3759,
    ENCHANT_BRACERS_FURL_FROST = 3760,
    ENCHANT_BRACERS_FURL_NATURE = 3762,
    ENCHANT_BRACERS_FURL_SHADOW = 3761,
    ENCHANT_BRACERS_FURL_ATTACK = 3756,
    ENCHANT_BRACERS_FURL_STAMINA = 3757,
    ENCHANT_BRACERS_FURL_SPELLPOWER = 3758,

    ENCHANT_CHEST_POWERFUL_STATS = 3832,
    ENCHANT_CHEST_SUPER_HEALTH = 3297,
    ENCHANT_CHEST_GREATER_MAINA_REST = 2381,
    ENCHANT_CHEST_EXCEPTIONAL_RESIL = 3245,
    ENCHANT_CHEST_GREATER_DEFENSE = 1953,

    ENCHANT_BOOTS_GREATER_ASSULT = 1597,
    ENCHANT_BOOTS_TUSKARS_VITLIATY = 3232,
    ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
    ENCHANT_BOOTS_GREATER_SPIRIT = 1147,
    ENCHANT_BOOTS_GREATER_VITALITY = 3244,
    ENCHANT_BOOTS_ICEWALKER = 3826,
    ENCHANT_BOOTS_GREATER_FORTITUDE = 1075,
    ENCHANT_BOOTS_NITRO_BOOTS = 3606,
    ENCHANT_BOOTS_PYRO_ROCKET = 3603,
    ENCHANT_BOOTS_ARMOR_WEBBING = 3860,

    ENCHANT_RING_ASSULT = 3839,
    ENCHANT_RING_GREATER_SP = 3840,
    ENCHANT_RING_STAMINA = 3791,
};

uint32 roll;
bool EnchanterEnableModule;
bool EnchanterAnnounceModule;
uint32 EnchanterNumPhrases;
uint32 EnchanterMessageTimer;
uint32 EnchanterEmoteSpell;
uint32 EnchanterEmoteCommand;

class EnchanterConfig : public WorldScript
{
public:
    EnchanterConfig() : WorldScript("EnchanterConfig_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            EnchanterEnableModule = sConfigMgr->GetOption<bool>("Enchanter.Enable", 1);
            EnchanterAnnounceModule = sConfigMgr->GetOption<bool>("Enchanter.Announce", 1);
            EnchanterNumPhrases = sConfigMgr->GetOption<uint32>("Enchanter.NumPhrases", 3);
            EnchanterMessageTimer = sConfigMgr->GetOption<uint32>("Enchanter.MessageTimer", 60000);
            EnchanterEmoteSpell = sConfigMgr->GetOption<uint32>("Enchanter.EmoteSpell", 44940);
            EnchanterEmoteCommand = sConfigMgr->GetOption<uint32>("Enchanter.EmoteCommand", 3);

            // Enforce Min/Max Time
            if (EnchanterMessageTimer != 0)
            {
                if (EnchanterMessageTimer < 60000 || EnchanterMessageTimer > 300000)
                {
                    EnchanterMessageTimer = 60000;
                }
            }
        }
    }
};

class EnchanterAnnounce : public PlayerScript
{

public:

    EnchanterAnnounce() : PlayerScript("EnchanterAnnounce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (EnchanterAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage("此服务器正在运行 |cff4CFF00 附魔师 NPC |r 模块。");
        }
    }
};

class npc_enchantment : public CreatureScript
{

public:

    npc_enchantment() : CreatureScript("npc_enchantment") { }

    // Pick Phrase
    static std::string PickPhrase()
    {
        // Choose and speak a random phrase to the player
        // Phrases are stored in the config file
        std::string phrase = "";
        uint32 PhraseNum = urand(1, EnchanterNumPhrases); // How many phrases does the NPC speak?
        phrase = "EC.P" + std::to_string(PhraseNum);

        // Sanitize
        if (phrase == "")
        {
            phrase = "错误! 未找到NPC说话文本,请检查配置文件!";
        }

        std::string randMsg = sConfigMgr->GetOption<std::string>(phrase.c_str(), "");
        return randMsg.c_str();
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {

        if (!EnchanterEnableModule)
        {
            return false;
        }

        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[附魔主手武器]", GOSSIP_SENDER_MAIN, 1);
        if (player->HasSpell(674))
        {
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[附魔副手武器]", GOSSIP_SENDER_MAIN, 13);
        }
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[附魔双手武器]", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[附魔盾牌]", GOSSIP_SENDER_MAIN, 3);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[附魔头盔]", GOSSIP_SENDER_MAIN, 4);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[附魔肩甲]", GOSSIP_SENDER_MAIN, 5);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[附魔披风]", GOSSIP_SENDER_MAIN, 6);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[附魔胸甲]", GOSSIP_SENDER_MAIN, 7);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[附魔护腕]", GOSSIP_SENDER_MAIN, 8);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[附魔手套]", GOSSIP_SENDER_MAIN, 9);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_pants_11:24:24:-18|t[附魔裤子]", GOSSIP_SENDER_MAIN, 10);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_boots_05:24:24:-18|t[附魔鞋子]", GOSSIP_SENDER_MAIN, 11);

        if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[附魔戒指]", GOSSIP_SENDER_MAIN, 12);

        player->PlayerTalkClass->SendGossipMenu(601015, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        if (!EnchanterEnableModule)
        {
            return false;
        }
    
        Item * item;
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {

        case 1: // 附魔主手武器
            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            {
                AddGossipItemFor(player, 1, "利刃防护", GOSSIP_SENDER_MAIN, 102);
                AddGossipItemFor(player, 1, "吸血", GOSSIP_SENDER_MAIN, 103);
            }
            AddGossipItemFor(player, 1, "26敏捷", GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 1, "45精神", GOSSIP_SENDER_MAIN, 101);
            AddGossipItemFor(player, 1, "狂暴", GOSSIP_SENDER_MAIN, 104);
            AddGossipItemFor(player, 1, "25命中+25暴击", GOSSIP_SENDER_MAIN, 105);
            AddGossipItemFor(player, 1, "黑魔法", GOSSIP_SENDER_MAIN, 106);
            AddGossipItemFor(player, 1, "作战专家", GOSSIP_SENDER_MAIN, 107);
            AddGossipItemFor(player, 1, "破冰武器", GOSSIP_SENDER_MAIN, 108);
            AddGossipItemFor(player, 1, "生命护卫", GOSSIP_SENDER_MAIN, 109);
            AddGossipItemFor(player, 1, "50耐力", GOSSIP_SENDER_MAIN, 110);
            AddGossipItemFor(player, 1, "65攻击强度", GOSSIP_SENDER_MAIN, 111);
            AddGossipItemFor(player, 1, "63法术强度", GOSSIP_SENDER_MAIN, 112);
            AddGossipItemFor(player, 1, "猫鼬", GOSSIP_SENDER_MAIN, 113);
            AddGossipItemFor(player, 1, "斩杀", GOSSIP_SENDER_MAIN, 114);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
            return true;
            break;

        case 2: // 附魔双手武器
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            if (!item)
            {
                creature->Whisper("这个附魔需要装备双手武器.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
            {
                AddGossipItemFor(player, 1, "狂暴", GOSSIP_SENDER_MAIN, 104);
                AddGossipItemFor(player, 1, "猫鼬", GOSSIP_SENDER_MAIN, 113);
                AddGossipItemFor(player, 1, "斩杀", GOSSIP_SENDER_MAIN, 114);
                AddGossipItemFor(player, 1, "81法术强度", GOSSIP_SENDER_MAIN, 115);
                AddGossipItemFor(player, 1, "35敏捷", GOSSIP_SENDER_MAIN, 116);
                AddGossipItemFor(player, 1, "110攻击强度", GOSSIP_SENDER_MAIN, 117);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("这个附魔需要装备双手武器.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
            }
            player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
            return true;
            break;

        case 3: // 附魔盾牌
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper("这个附魔需要装备盾牌.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
            {
                AddGossipItemFor(player, 1, "20防御等级", GOSSIP_SENDER_MAIN, 118);
                AddGossipItemFor(player, 1, "25智力", GOSSIP_SENDER_MAIN, 119);
                AddGossipItemFor(player, 1, "12韧性等级", GOSSIP_SENDER_MAIN, 120);
                AddGossipItemFor(player, 1, "泰坦神铁护板", GOSSIP_SENDER_MAIN, 121);
                AddGossipItemFor(player, 1, "18耐力", GOSSIP_SENDER_MAIN, 122);
                AddGossipItemFor(player, 1, "36格挡值", GOSSIP_SENDER_MAIN, 123);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("这个附魔需要装备盾牌.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 4: // 附魔头盔
            AddGossipItemFor(player, 1, "30法术强度+5秒10法力回复", GOSSIP_SENDER_MAIN, 124);
            AddGossipItemFor(player, 1, "30法术强度+20暴击等级", GOSSIP_SENDER_MAIN, 125);
            AddGossipItemFor(player, 1, "29法术强度+20韧性等级", GOSSIP_SENDER_MAIN, 126);
            AddGossipItemFor(player, 1, "30耐力+25韧性等级", GOSSIP_SENDER_MAIN, 127);
            AddGossipItemFor(player, 1, "37耐力+20防御等级", GOSSIP_SENDER_MAIN, 128);
            AddGossipItemFor(player, 1, "50攻击强度+20暴击等级", GOSSIP_SENDER_MAIN, 129);
            AddGossipItemFor(player, 1, "50攻击强度+20韧性等级", GOSSIP_SENDER_MAIN, 130);
            AddGossipItemFor(player, 1, "25奥术抗性+30耐力", GOSSIP_SENDER_MAIN, 131);
            AddGossipItemFor(player, 1, "25火焰抗性+30耐力", GOSSIP_SENDER_MAIN, 132);
            AddGossipItemFor(player, 1, "25暗影抗性+30耐力", GOSSIP_SENDER_MAIN, 133);
            AddGossipItemFor(player, 1, "25冰霜抗性+30耐力", GOSSIP_SENDER_MAIN, 134);
            AddGossipItemFor(player, 1, "25自然抗性+30耐力", GOSSIP_SENDER_MAIN, 135);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
            return true;
            break;

        case 5: // 附魔肩甲
            if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
            {
                AddGossipItemFor(player, 1, "120攻击强度+15暴击等级", GOSSIP_SENDER_MAIN, 136);
                AddGossipItemFor(player, 1, "70法术强度+5秒8法力回复", GOSSIP_SENDER_MAIN, 137);
                AddGossipItemFor(player, 1, "60躲闪等级+15防御等级", GOSSIP_SENDER_MAIN, 138);
                AddGossipItemFor(player, 1, "70法术强度+15韧性等级", GOSSIP_SENDER_MAIN, 139);
            }
            AddGossipItemFor(player, 1, "40攻击强度+15暴击等级", GOSSIP_SENDER_MAIN, 140);
            AddGossipItemFor(player, 1, "24法术强度+5秒8法力回复", GOSSIP_SENDER_MAIN, 141);
            AddGossipItemFor(player, 1, "30耐力+15韧性等级", GOSSIP_SENDER_MAIN, 142);
            AddGossipItemFor(player, 1, "20躲闪等级+15防御等级", GOSSIP_SENDER_MAIN, 143);
            AddGossipItemFor(player, 1, "24法术强度+15暴击等级", GOSSIP_SENDER_MAIN, 144);
            AddGossipItemFor(player, 1, "23法术强度+15韧性等级", GOSSIP_SENDER_MAIN, 145);
            AddGossipItemFor(player, 1, "40攻击强度+15韧性等级", GOSSIP_SENDER_MAIN, 146);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
            return true;
            break;

        case 6: // 附魔披风
            if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
            {
                AddGossipItemFor(player, 1, "黑光刺绣", GOSSIP_SENDER_MAIN, 149);
                AddGossipItemFor(player, 1, "亮纹刺绣", GOSSIP_SENDER_MAIN, 150);
                AddGossipItemFor(player, 1, "剑刃刺绣", GOSSIP_SENDER_MAIN, 151);
            }
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddGossipItemFor(player, 1, "降落伞", GOSSIP_SENDER_MAIN, 147);
            }
            AddGossipItemFor(player, 1, "强化潜行+10敏捷", GOSSIP_SENDER_MAIN, 148);
            AddGossipItemFor(player, 1, "10精神+威胁降低2%", GOSSIP_SENDER_MAIN, 152);
            AddGossipItemFor(player, 1, "16防御等级", GOSSIP_SENDER_MAIN, 153);
            AddGossipItemFor(player, 1, "35法术穿透", GOSSIP_SENDER_MAIN, 154);
            AddGossipItemFor(player, 1, "225护甲值", GOSSIP_SENDER_MAIN, 155);
            AddGossipItemFor(player, 1, "22敏捷", GOSSIP_SENDER_MAIN, 156);
            AddGossipItemFor(player, 1, "23急速", GOSSIP_SENDER_MAIN, 157);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
            return true;
            break;

        case 7: //附魔胸甲
            AddGossipItemFor(player, 1, "+10所有属性", GOSSIP_SENDER_MAIN, 158);
            AddGossipItemFor(player, 1, "275生命值", GOSSIP_SENDER_MAIN, 159);
            AddGossipItemFor(player, 1, "5秒8法力回复", GOSSIP_SENDER_MAIN, 160);
            AddGossipItemFor(player, 1, "20韧性等级", GOSSIP_SENDER_MAIN, 161);
            AddGossipItemFor(player, 1, "22防御等级", GOSSIP_SENDER_MAIN, 162);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
            return true;
            break;

        case 8: //附魔护腕
            AddGossipItemFor(player, 1, "40耐力", GOSSIP_SENDER_MAIN, 163);
            AddGossipItemFor(player, 1, "30法术强度", GOSSIP_SENDER_MAIN, 164);
            AddGossipItemFor(player, 1, "50攻击强度", GOSSIP_SENDER_MAIN, 165);
            AddGossipItemFor(player, 1, "18精神", GOSSIP_SENDER_MAIN, 166);
            AddGossipItemFor(player, 1, "15精淮等级", GOSSIP_SENDER_MAIN, 167);
            AddGossipItemFor(player, 1, "+6所有属性", GOSSIP_SENDER_MAIN, 168);
            AddGossipItemFor(player, 1, "16智力", GOSSIP_SENDER_MAIN, 169);
            if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
            {
                AddGossipItemFor(player, 1, "70奥术抗性", GOSSIP_SENDER_MAIN, 170);
                AddGossipItemFor(player, 1, "70火焰抗性", GOSSIP_SENDER_MAIN, 171);
                AddGossipItemFor(player, 1, "70冰霜抗性", GOSSIP_SENDER_MAIN, 172);
                AddGossipItemFor(player, 1, "70自然抗性", GOSSIP_SENDER_MAIN, 173);
                AddGossipItemFor(player, 1, "70暗影抗性", GOSSIP_SENDER_MAIN, 174);
                AddGossipItemFor(player, 1, "130攻击强度", GOSSIP_SENDER_MAIN, 175);
                AddGossipItemFor(player, 1, "102耐力", GOSSIP_SENDER_MAIN, 176);
                AddGossipItemFor(player, 1, "76法术强度", GOSSIP_SENDER_MAIN, 177);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
            return true;
            break;

        case 9: //附魔手套
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddGossipItemFor(player, 1, "超级加速器", GOSSIP_SENDER_MAIN, 200);
            }
            AddGossipItemFor(player, 1, "16爆击等级", GOSSIP_SENDER_MAIN, 178);
            AddGossipItemFor(player, 1, "2%威胁+10招架等级", GOSSIP_SENDER_MAIN, 179);
            AddGossipItemFor(player, 1, "44攻击强度", GOSSIP_SENDER_MAIN, 180);
            AddGossipItemFor(player, 1, "20敏捷", GOSSIP_SENDER_MAIN, 181);
            AddGossipItemFor(player, 1, "20命中等级", GOSSIP_SENDER_MAIN, 182);
            AddGossipItemFor(player, 1, "15精淮等级", GOSSIP_SENDER_MAIN, 183);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
            return true;
            break;

        case 10: //附魔裤子
            AddGossipItemFor(player, 1, "40韧性等级+28耐力", GOSSIP_SENDER_MAIN, 184);
            AddGossipItemFor(player, 1, "55耐力+22敏捷", GOSSIP_SENDER_MAIN, 185);
            AddGossipItemFor(player, 1, "75攻击强度+22爆击等级", GOSSIP_SENDER_MAIN, 186);
            AddGossipItemFor(player, 1, "50法术强度+20精神", GOSSIP_SENDER_MAIN, 187);
            AddGossipItemFor(player, 1, "50法术强度+30耐力", GOSSIP_SENDER_MAIN, 188);
            AddGossipItemFor(player, 1, "72耐力+35敏捷", GOSSIP_SENDER_MAIN, 189);
            AddGossipItemFor(player, 1, "100攻击强度+36爆击等级", GOSSIP_SENDER_MAIN, 190);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
            return true;
            break;

        case 11: //附魔鞋子
            AddGossipItemFor(player, 1, "32攻击强度", GOSSIP_SENDER_MAIN, 191);
            AddGossipItemFor(player, 1, "15耐力+速度略微提高", GOSSIP_SENDER_MAIN, 192);
            AddGossipItemFor(player, 1, "16敏捷", GOSSIP_SENDER_MAIN, 193);
            AddGossipItemFor(player, 1, "18精神", GOSSIP_SENDER_MAIN, 194);
            AddGossipItemFor(player, 1, "+7生命+5秒7法力回复", GOSSIP_SENDER_MAIN, 195);
            AddGossipItemFor(player, 1, "12命中+12爆击等级", GOSSIP_SENDER_MAIN, 196);
            AddGossipItemFor(player, 1, "22耐力", GOSSIP_SENDER_MAIN, 197);
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddGossipItemFor(player, 1, "地精火箭", GOSSIP_SENDER_MAIN, 198);
                AddGossipItemFor(player, 1, "火箭发射", GOSSIP_SENDER_MAIN, 199);
                AddGossipItemFor(player, 1, "885护甲值", GOSSIP_SENDER_MAIN, 201);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
            return true;
            break;

        case 12: //Enchant rings
            AddGossipItemFor(player, 1, "40攻击强度", GOSSIP_SENDER_MAIN, 202);
            AddGossipItemFor(player, 1, "23法术强度", GOSSIP_SENDER_MAIN, 203);
            AddGossipItemFor(player, 1, "30耐力", GOSSIP_SENDER_MAIN, 204);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
            return true;
            break;

        case 13: // 附魔副手武器
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper("这个附魔需要装备副手武器.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
            {
                if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                {
                    AddGossipItemFor(player, 1, "利刃防护", GOSSIP_SENDER_MAIN, 207);
                    AddGossipItemFor(player, 1, "吸血", GOSSIP_SENDER_MAIN, 208);
                }
                AddGossipItemFor(player, 1, "26敏捷", GOSSIP_SENDER_MAIN, 205);
                AddGossipItemFor(player, 1, "45精神", GOSSIP_SENDER_MAIN, 206);
                AddGossipItemFor(player, 1, "狂暴", GOSSIP_SENDER_MAIN, 209);
                AddGossipItemFor(player, 1, "25命中25暴击", GOSSIP_SENDER_MAIN, 210);
                AddGossipItemFor(player, 1, "黑魔法", GOSSIP_SENDER_MAIN, 211);
                AddGossipItemFor(player, 1, "作战专家", GOSSIP_SENDER_MAIN, 212);
                AddGossipItemFor(player, 1, "破冰武器", GOSSIP_SENDER_MAIN, 213);
                AddGossipItemFor(player, 1, "生命护卫", GOSSIP_SENDER_MAIN, 214);
                AddGossipItemFor(player, 1, "50耐力", GOSSIP_SENDER_MAIN, 215);
                AddGossipItemFor(player, 1, "65攻击强度", GOSSIP_SENDER_MAIN, 216);
                AddGossipItemFor(player, 1, "63法术强度", GOSSIP_SENDER_MAIN, 217);
                AddGossipItemFor(player, 1, "猫鼬", GOSSIP_SENDER_MAIN, 218);
                AddGossipItemFor(player, 1, "斩杀", GOSSIP_SENDER_MAIN, 219);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "返回", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("这个附魔需要装备副手武器.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 100:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 101:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 102:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 103:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 104:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 105:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 106:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 107:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 108:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 109:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 110:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 111:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
            break;

        case 112:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 113:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 114:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
            break;

        case 115:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
            break;

        case 116:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
            break;

        case 117:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
            break;

        case 118:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
            break;

        case 119:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
            break;

        case 120:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
            break;

        case 121:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
            break;

        case 122:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
            break;

        case 123:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
            break;

        case 124:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
            break;

        case 125:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
            break;

        case 126:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
            break;

        case 127:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
            break;

        case 128:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
            break;

        case 129:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
            break;

        case 130:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
            break;

        case 131:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
            break;

        case 132:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
            break;

        case 133:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
            break;

        case 134:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
            break;

        case 135:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
            break;

        case 136:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
            break;

        case 137:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
            break;

        case 138:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
            break;

        case 139:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
            break;

        case 140:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
            break;

        case 141:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
            break;

        case 142:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
            break;

        case 143:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
            break;

        case 144:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
            break;

        case 145:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
            break;

        case 146:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
            break;

        case 147:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
            break;

        case 148:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
            break;

        case 149:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
            break;

        case 150:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
            break;

        case 151:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
            break;

        case 152:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
            break;

        case 153:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
            break;

        case 154:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
            break;

        case 155:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
            break;

        case 156:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
            break;

        case 157:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
            break;

        case 158:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
            break;

        case 159:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
            break;

        case 160:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
            break;

        case 161:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
            break;

        case 162:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
            break;

        case 163:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
            break;

        case 164:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
            break;

        case 165:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
            break;

        case 166:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
            break;

        case 167:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
            break;

        case 168:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_STATS);
            break;

        case 169:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
            break;

        case 170:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
            break;

        case 171:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
            break;

        case 172:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
            break;

        case 173:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
            break;

        case 174:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
            break;

        case 175:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
            break;

        case 176:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
            break;

        case 177:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
            break;

        case 178:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
            break;

        case 179:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
            break;

        case 180:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
            break;

        case 181:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
            break;

        case 182:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
            break;

        case 183:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXPERTISE);
            break;

        case 184:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
            break;

        case 185:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
            break;

        case 186:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
            break;

        case 187:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
            break;

        case 188:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
            break;

        case 189:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
            break;

        case 190:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
            break;

        case 191:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
            break;

        case 192:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
            break;

        case 193:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
            break;

        case 194:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
            break;

        case 195:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
            break;

        case 196:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
            break;

        case 197:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
            break;

        case 198:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
            break;

        case 199:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_PYRO_ROCKET);
            break;

        case 200:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HYPERSPEED);
            break;

        case 201:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ARMOR_WEBBING);
            break;

        case 202:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
            break;

        case 203:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
            break;

        case 204:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
            break;

        case 205:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 206:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 207:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 208:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 209:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 210:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 211:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 212:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 213:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 214:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 215:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 216:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
            break;

        case 217:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 218:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 219:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
            break;
        
        case 300:
        {
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[附魔主手武器]", GOSSIP_SENDER_MAIN, 1);
            if (player->HasSpell(674))
            {
                AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[附魔副手武器]", GOSSIP_SENDER_MAIN, 13);
            }
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[附魔双手武器]", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[附魔盾牌]", GOSSIP_SENDER_MAIN, 3);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[附魔头盔]", GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[附魔肩甲]", GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[附魔披风]", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[附魔胸甲]", GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[附魔护腕]", GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[附魔手套]", GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_pants_11:24:24:-18|t[附魔裤子]", GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_boots_05:24:24:-18|t[附魔鞋子]", GOSSIP_SENDER_MAIN, 11);

            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[附魔戒指]", GOSSIP_SENDER_MAIN, 12);

            player->PlayerTalkClass->SendGossipMenu(601015, creature->GetGUID());
            return true;
            break;
        }
        }

        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    void Enchant(Player* player, Creature* creature, Item* item, uint32 enchantid)
{
    if (!item)
    {
        creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
        creature->Whisper("请装备你想要附魔的物品!", LANG_UNIVERSAL, player);
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (!enchantid)
    {
        ChatHandler(player->GetSession()).SendNotification("代码出了问题。我们已将此问题记录在案，并将对开发人员进行调查，给您带来的不便深表歉意.");
        player->PlayerTalkClass->SendCloseGossip();
        creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
        return;
    }

    // 获取物品模板
    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item->GetTemplate()->ItemId);
    if (!pProto)
    {
        ChatHandler(player->GetSession()).SendNotification("无法获取物品模板!");
        return;
    }

    // 获取物品本地化名称
    std::string itemName = pProto->Name1;  // 默认名称

    // 获取物品的本地化名称，如果物品有本地化数据
    if (ItemLocale const* il = sObjectMgr->GetItemLocale(item->GetTemplate()->ItemId))
    {
        // 使用 LOCALE_zhCN 获取中文名称
        ObjectMgr::GetLocaleString(il->Name, LOCALE_zhCN, itemName);
    }

    // Roll the dice
    uint32 roll = urand(1, 100);

    item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
    item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);

    // 随机附魔通知
    std::string message;
    if (roll > 0 && roll < 33)
    {
        message = "|cff00ff00博瑞德的枯骨手指触碰 |cffDA70D6" + itemName + " |cff00ff00时闪耀着能量！";
        ChatHandler(player->GetSession()).SendNotification(message.c_str());
    }
    else if (roll > 33 && roll < 75)
    {
        message = "|cff00ff00博瑞德举起 |cffDA70D6" + itemName + " |cff00ff00高高在空中，念起了奇怪的咒语！";
        ChatHandler(player->GetSession()).SendNotification(message.c_str());
    }
    else
    {
        message = "|cff00ff00博瑞德深入集中注意力，用他的魔杖在 |cffDA70D6" + itemName + " |cff00ff00上挥舞！";
        ChatHandler(player->GetSession()).SendNotification(message.c_str());
    }

    // 施放附魔特效
    creature->CastSpell(player, 12512); // enchantment visual
    player->PlayerTalkClass->SendCloseGossip();
}



    // Passive Emotes
    struct NPC_PassiveAI : public ScriptedAI
    {
        NPC_PassiveAI(Creature * creature) : ScriptedAI(creature) { }

        uint32 MessageTimer = 0;

        // Called once when client is loaded
        void Reset()
        {
            if (EnchanterMessageTimer != 0)
            {
                MessageTimer = urand(EnchanterMessageTimer, 300000); // 1-5 minutes
            }
        }

        // Called at World update tick
        void UpdateAI(const uint32 diff)
        {
            // If Enabled
            if (EnchanterEnableModule && EnchanterMessageTimer != 0)
            {
                if (MessageTimer <= diff)
                {
                    if (EnchanterNumPhrases > 0)
                    {
                        std::string Message = PickPhrase();
                        me->Say(Message.c_str(), LANG_UNIVERSAL);
                    }

                    // Use gesture?
                    if (EnchanterEmoteCommand != 0)
                    {
                        me->HandleEmoteCommand(EnchanterEmoteCommand);
                    }

                    // Alert players?
                    if (EnchanterEmoteSpell != 0)
                    {
                        me->CastSpell(me, EnchanterEmoteSpell);
                    }

                    MessageTimer = urand(EnchanterMessageTimer, 300000);
                }
                else { MessageTimer -= diff; }
            }
        }
    };

    // CREATURE AI
    CreatureAI * GetAI(Creature * creature) const
    {
        return new NPC_PassiveAI(creature);
    }
};

void AddNPCEnchanterScripts()
{
    new EnchanterConfig();
    new EnchanterAnnounce();
    new npc_enchantment();
}
