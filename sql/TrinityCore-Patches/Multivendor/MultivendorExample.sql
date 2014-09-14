

REPLACE INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(190017, 1298, "Herbert", "MultiVendor", NULL, @MENU_ID, 10, 10, 0, 35, 129, 1, 1.14286, 1, 0, 13, 17, 0, 42, 1, 1500, 0, 1, 512, 2048, 8, 0, 0, 0, 0, 0, 9, 13, 100, 7, 138412032, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 1, 0, 2, '');

REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(@MENU_ID, 0, 4, 'VendorTest 465', 3, 128, 465, 0, 0, 0, ''),
(@MENU_ID, 1, 9, 'VendorTest 54', 3, 128, 54, 0, 0, 0, ''),
(@MENU_ID, 2, 6, 'VendorTest 35574', 3, 128, 35574, 0, 0, 100, 'These goods are special, so pay up!');
