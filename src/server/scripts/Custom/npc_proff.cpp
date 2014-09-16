#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Language.h"



#define INCOMBAT "Вы находитесь в бою."
#define INPROFF "ProffNPC: received non-valid skill ID (LearnAllRecipesInProfession)"

#define TEXT_PROFFESION_SECON_MENU "[Меню Второстепенных профессий]"
#define TEXT_MAIN_MENU "[Вернутся в Главное меню]"

#define TEXT_ALCHEMY " Алхимия."
#define TEXT_BLACKSMITHING " Кузнечное Дело."
#define TEXT_ENCNANTING " Наложение Чар."
#define TEXT_ENGINEERING " Инженерное дело."
#define TEXT_HERBALISM " Травничество."
#define TEXT_INSCRIPTION " Начертание."
#define TEXT_JEWELCRAFTING " Ювелирное дело."
#define TEXT_LEATHERWORKING " Кожевничество."
#define TEXT_MINING " Горное дело."
#define TEXT_SKINNING " Снятие шкур."
#define TEXT_TAILORING " Портняжное дело."

#define TEXT_COOKING	" Кулинария."
#define TEXT_FIRST_AID " Первая помощь."
#define TEXT_FISHING	" Рыбная ловля."

class npc_proff : public CreatureScript
{
public:
	npc_proff() : CreatureScript("npc_proff") { }

	void CompleteLearnProfession(Player *player, Creature * creature, SkillType skill)
	{
		if (PlayerAlreadyHasNineProfessions(player) && !IsSecondarySkill(skill))
			creature->Whisper("Вы уже выучили 2 професcии!", LANG_UNIVERSAL, player);
		else
		{
			if (!LearnAllRecipesInProfession(player, skill))
				creature->Whisper("Ошибка!", LANG_UNIVERSAL, player);
		}
	}

	bool IsSecondarySkill(SkillType skill) const
	{
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING;
	}

	bool PlayerAlreadyHasNineProfessions(const Player *player) const
	{
		uint32 skillCount = 0;

		if (player->HasSkill(SKILL_MINING))
			skillCount++;
		if (player->HasSkill(SKILL_SKINNING))
			skillCount++;
		if (player->HasSkill(SKILL_HERBALISM))
			skillCount++;

		if (skillCount >= 2)
			return true;

		for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
		{
			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
			if (!SkillInfo)
				continue;

			if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
				continue;

			if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
				continue;

			const uint32 skillID = SkillInfo->id;
			if (player->HasSkill(skillID))
				skillCount++;

			if (skillCount >= 2)
				return true;
		}

		return false;
	}

	bool LearnAllRecipesInProfession(Player *player, SkillType skill)
	{
		ChatHandler handler(player->GetSession());
		char* skill_name;

		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
		skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];

		if (!SkillInfo)
		{
			TC_LOG_ERROR("entities.player", INPROFF);
			return false;
		}

		LearnSkillRecipesHelper(player, SkillInfo->id);

		uint16 maxLevel = player->GetPureMaxSkillValue(SkillInfo->id);
		player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
		handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);

		return true;
	}
	// See "static void HandleLearnSkillRecipesHelper(Player* player,uint32 skill_id)" from cs_learn.cpp  
	void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
	{
		uint32 classmask = player->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
		{
			SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
			if (!skillLine)
				continue;

			// wrong skill 
			if (skillLine->skillId != skill_id)
				continue;

			// not high rank 
			if (skillLine->forward_spellid)
				continue;

			// skip racial skills 
			if (skillLine->racemask != 0)
				continue;

			// skip wrong class skills 
			if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
				continue;

			SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
			if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
				continue;

			player->LearnSpell(skillLine->spellId, false);
		}
	}
	bool OnGossipHello(Player *player, Creature *creature)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_ALCHEMY, GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_BLACKSMITHING, GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_ENCNANTING, GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_ENGINEERING, GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_HERBALISM, GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_INSCRIPTION, GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_JEWELCRAFTING, GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_LEATHERWORKING, GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_MINING, GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_SKINNING, GOSSIP_SENDER_MAIN, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,TEXT_TAILORING, GOSSIP_SENDER_MAIN, 12);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK,TEXT_PROFFESION_SECON_MENU, GOSSIP_SENDER_MAIN, 13);
	     player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		 return true;
	 }
	 bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action)
		 {
		  if (!player->getAttackers().empty())
	 {
		creature->Whisper(INCOMBAT, LANG_UNIVERSAL, player);
		player->CLOSE_GOSSIP_MENU();
        return false;
		  }
		  switch (action)
	{
	case 2:
		CompleteLearnProfession(player, creature, SKILL_ALCHEMY); 

		player->CLOSE_GOSSIP_MENU();

		break;
	case 3:
		CompleteLearnProfession(player, creature, SKILL_BLACKSMITHING);
 
		player->CLOSE_GOSSIP_MENU();

		break;
	case 4:
		CompleteLearnProfession(player, creature,  SKILL_ENCHANTING); 
		player->CLOSE_GOSSIP_MENU();
		break;
	case 5:
		CompleteLearnProfession(player, creature, SKILL_ENGINEERING); 
		player->CLOSE_GOSSIP_MENU();
		break;
	case 6:
		CompleteLearnProfession(player, creature, SKILL_HERBALISM); 
		player->CLOSE_GOSSIP_MENU();
		break;
	case 7:
		CompleteLearnProfession(player, creature, SKILL_INSCRIPTION); 
		player->CLOSE_GOSSIP_MENU();
		break;
	case 8:
		CompleteLearnProfession(player, creature, SKILL_JEWELCRAFTING);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 9:
		CompleteLearnProfession(player, creature, SKILL_LEATHERWORKING);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 10:
		CompleteLearnProfession(player, creature, SKILL_MINING);
		player->CLOSE_GOSSIP_MENU();
	    break;
	case 11:
		CompleteLearnProfession(player, creature, SKILL_SKINNING);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 12:
		CompleteLearnProfession(player, creature, SKILL_TAILORING);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 13:
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, TEXT_COOKING, GOSSIP_SENDER_MAIN, 14);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, TEXT_FIRST_AID, GOSSIP_SENDER_MAIN, 15);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, TEXT_FISHING, GOSSIP_SENDER_MAIN, 16);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, TEXT_MAIN_MENU, GOSSIP_SENDER_MAIN, 17);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		break;
	case 14:
		CompleteLearnProfession(player, creature, SKILL_COOKING);
		player->CLOSE_GOSSIP_MENU();
		break;
	case 15:
		CompleteLearnProfession(player, creature, SKILL_FIRST_AID); 
		player->CLOSE_GOSSIP_MENU();
	    break;
	case 16:
		CompleteLearnProfession(player, creature, SKILL_FISHING); 
		player->CLOSE_GOSSIP_MENU();
	    break;
	case 17:
		OnGossipHello(player, creature);
		break;
	 }
		  return true;
	 }
	 
	};
 void AddSC_npc_proff()
{
    new npc_proff;
}