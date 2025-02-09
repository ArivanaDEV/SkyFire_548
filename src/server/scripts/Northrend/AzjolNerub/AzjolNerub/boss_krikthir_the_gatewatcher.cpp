/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
 * Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Comment: Find in the future best timers and the event is not implemented.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "azjol_nerub.h"

enum Spells
{
    SPELL_MIND_FLAY                               = 52586,
    H_SPELL_MIND_FLAY                             = 59367,
    SPELL_CURSE_OF_FATIGUE                        = 52592,
    H_SPELL_CURSE_OF_FATIGUE                      = 59368,
    SPELL_FRENZY                                  = 28747, //maybe 53361
    SPELL_SUMMON_SKITTERING_SWARMER               = 52438, //AOE Effect 140, maybe 52439
    SPELL_SUMMON_SKITTERING_SWARMER_1             = 52439, //Summon 3x 28735
    H_SPELL_ACID_SPLASH                           = 59363,
    SPELL_ACID_SPLASH                             = 52446,
    SPELL_CHARGE                                  = 16979, //maybe is another spell
    SPELL_BACKSTAB                                = 52540,
    SPELL_SHADOW_BOLT                             = 52534,
    H_SPELL_SHADOW_BOLT                           = 59357,
    SPELL_SHADOW_NOVA                             = 52535,
    H_SPELL_SHADOW_NOVA                           = 59358,
    SPELL_STRIKE                                  = 52532,
    SPELL_CLEAVE                                  = 49806,
    SPELL_ENRAGE                                  = 52470,
    SPELL_INFECTED_BITE                           = 52469,
    H_SPELL_INFECTED_BITE                         = 59364,
    SPELL_WEB_WRAP                                = 52086, //the spell is not working properly
    SPELL_BLINDING_WEBS                           = 52524,
    H_SPELL_BLINDING_WEBS                         = 59365,
    SPELL_POSION_SPRAY                            = 52493,
    H_SPELL_POSION_SPRAY                          = 59366
};

enum Mobs
{
    NPC_SKITTERING_SWARMER                        = 28735,
    NPC_SKITTERING_SWARMER_CONTROLLER             = 32593,
    NPC_SKITTERING_INFECTIOR                      = 28736
};

enum Yells
{
    SAY_AGGRO                                     = 0,
    SAY_SLAY                                      = 1,
    SAY_DEATH                                     = 2,
    SAY_SWARM                                     = 3,
    SAY_PREFIGHT                                  = 4,
    SAY_SEND_GROUP                                = 5
};

const Position SpawnPoint[] =
{
    { 566.164f, 682.087f, 769.079f, 2.21657f  },
    { 529.042f, 706.941f, 777.298f, 1.0821f   },
    { 489.975f, 671.239f, 772.131f, 0.261799f },
    { 488.556f, 692.95f,  771.764f, 4.88692f  },
    { 553.34f,  640.387f, 777.419f, 1.20428f  },
    { 517.486f, 706.398f, 777.335f, 5.35816f  },
    { 504.01f,  637.693f, 777.479f, 0.506145f },
    { 552.625f, 706.408f, 777.177f, 3.4383f   }
};

class boss_krik_thir : public CreatureScript
{
public:
    boss_krik_thir() : CreatureScript("boss_krik_thir") { }

    struct boss_krik_thirAI : public ScriptedAI
    {
        boss_krik_thirAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiMindFlayTimer;
        uint32 uiCurseFatigueTimer;
        uint32 uiSummonTimer;

        void Reset() OVERRIDE
        {
            uiMindFlayTimer = 15*IN_MILLISECONDS;
            uiCurseFatigueTimer = 12*IN_MILLISECONDS;

            if (instance)
                instance->SetBossState(DATA_KRIKTHIR_THE_GATEWATCHER, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            Summon();
            uiSummonTimer = 15*IN_MILLISECONDS;

            if (instance)
                instance->SetBossState(DATA_KRIKTHIR_THE_GATEWATCHER, IN_PROGRESS);
        }

        void Summon()
        {
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[0], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[0], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[1], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[1], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[2], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[2], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[3], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[3], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_INFECTIOR, SpawnPoint[4], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[4], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_INFECTIOR, SpawnPoint[5], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[5], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_INFECTIOR, SpawnPoint[6], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[6], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[7], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
                me->SummonCreature(NPC_SKITTERING_SWARMER, SpawnPoint[7], TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 25*IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiSummonTimer <= diff)
            {
                Summon();
                uiSummonTimer = 15*IN_MILLISECONDS;
            } else uiSummonTimer -= diff;

            if (uiMindFlayTimer <= diff)
            {
                    DoCastVictim(SPELL_MIND_FLAY);
                    uiMindFlayTimer = 15*IN_MILLISECONDS;
                } else uiMindFlayTimer -= diff;

            if (uiCurseFatigueTimer <= diff)
            {
                //WowWiki say "Curse of Fatigue-Kirk'thir will cast Curse of Fatigue on 2-3 targets periodically."
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CURSE_OF_FATIGUE);
                if (Unit* tankTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                    DoCast(tankTarget, SPELL_CURSE_OF_FATIGUE);

                uiCurseFatigueTimer = 10*IN_MILLISECONDS;
            } else uiCurseFatigueTimer -= diff;

            if (!me->HasAura(SPELL_FRENZY) && HealthBelowPct(10))
                DoCast(me, SPELL_FRENZY, true);

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetBossState(DATA_KRIKTHIR_THE_GATEWATCHER, DONE);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Talk(SAY_SLAY);
        }

        void JustSummoned(Creature* summoned) OVERRIDE
        {
            summoned->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_krik_thirAI(creature);
    }
};

class npc_skittering_infector : public CreatureScript
{
public:
    npc_skittering_infector() : CreatureScript("npc_skittering_infector") { }

    struct npc_skittering_infectorAI : public ScriptedAI
    {
        npc_skittering_infectorAI(Creature* creature) : ScriptedAI(creature) { }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            //The spell is not working propperly
            DoCastVictim(SPELL_ACID_SPLASH, true);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_skittering_infectorAI(creature);
    }
};

class npc_anub_ar_skirmisher : public CreatureScript
{
public:
    npc_anub_ar_skirmisher() : CreatureScript("npc_anub_ar_skirmisher") { }

    struct npc_anub_ar_skirmisherAI : public ScriptedAI
    {
        npc_anub_ar_skirmisherAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiChargeTimer;
        uint32 uiBackstabTimer;

        void Reset() OVERRIDE
        {
            uiChargeTimer   = 11*IN_MILLISECONDS;
            uiBackstabTimer = 7*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiChargeTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                {
                    DoResetThreat();
                    me->AddThreat(target, 1.0f);
                    DoCast(target, SPELL_CHARGE, true);
                }
                uiChargeTimer = 15*IN_MILLISECONDS;
            } else uiChargeTimer -= diff;

            if (uiBackstabTimer <= diff)
            {
                DoCastVictim(SPELL_BACKSTAB);
                uiBackstabTimer = 12*IN_MILLISECONDS;
            } else uiBackstabTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_anub_ar_skirmisherAI(creature);
    }
};

class npc_anub_ar_shadowcaster : public CreatureScript
{
public:
    npc_anub_ar_shadowcaster() : CreatureScript("npc_anub_ar_shadowcaster") { }

    struct npc_anub_ar_shadowcasterAI : public ScriptedAI
    {
        npc_anub_ar_shadowcasterAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiShadowBoltTimer;
        uint32 uiShadowNovaTimer;

        void Reset() OVERRIDE
        {
            uiShadowBoltTimer = 6*IN_MILLISECONDS;
            uiShadowNovaTimer = 15*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiShadowBoltTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                     DoCast(target, SPELL_SHADOW_BOLT, true);
                uiShadowBoltTimer = 15*IN_MILLISECONDS;
            } else uiShadowBoltTimer -= diff;

            if (uiShadowNovaTimer <= diff)
            {
                DoCastVictim(SPELL_SHADOW_NOVA, true);
                uiShadowNovaTimer = 17*IN_MILLISECONDS;
            } else uiShadowNovaTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_anub_ar_shadowcasterAI(creature);
    }
};

class npc_anub_ar_warrior : public CreatureScript
{
public:
    npc_anub_ar_warrior() : CreatureScript("npc_anub_ar_warrior") { }

    struct npc_anub_ar_warriorAI : public ScriptedAI
    {
        npc_anub_ar_warriorAI(Creature* creature) : ScriptedAI(creature){ }

        uint32 uiCleaveTimer;
        uint32 uiStrikeTimer;

        void Reset() OVERRIDE
        {
            uiCleaveTimer = 11*IN_MILLISECONDS;
            uiStrikeTimer = 6*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiStrikeTimer <= diff)
            {
                DoCastVictim(SPELL_STRIKE, true);
                uiStrikeTimer = 15*IN_MILLISECONDS;
            } else uiStrikeTimer -= diff;

            if (uiCleaveTimer <= diff)
            {
                DoCastVictim(SPELL_CLEAVE, true);
                uiCleaveTimer = 17*IN_MILLISECONDS;
            } else uiCleaveTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_anub_ar_warriorAI(creature);
    }
};

class npc_watcher_gashra : public CreatureScript
{
public:
    npc_watcher_gashra() : CreatureScript("npc_watcher_gashra") { }

    struct npc_watcher_gashraAI : public ScriptedAI
    {
        npc_watcher_gashraAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiWebWrapTimer;
        uint32 uiInfectedBiteTimer;

        void Reset() OVERRIDE
        {
            uiWebWrapTimer      = 11*IN_MILLISECONDS;
            uiInfectedBiteTimer = 4*IN_MILLISECONDS;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            DoCast(me, SPELL_ENRAGE, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiWebWrapTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_WEB_WRAP, true);
                uiWebWrapTimer = 17*IN_MILLISECONDS;
            } else uiWebWrapTimer -= diff;

            if (uiInfectedBiteTimer <= diff)
            {
                DoCastVictim(SPELL_INFECTED_BITE, true);
                uiInfectedBiteTimer = 15*IN_MILLISECONDS;
            } else uiInfectedBiteTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_watcher_gashraAI(creature);
    }
};

class npc_watcher_narjil : public CreatureScript
{
public:
    npc_watcher_narjil() : CreatureScript("npc_watcher_narjil") { }

    struct npc_watcher_narjilAI : public ScriptedAI
    {
        npc_watcher_narjilAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiWebWrapTimer;
        uint32 uiInfectedBiteTimer;
        uint32 uiBindingWebsTimer;

        void Reset() OVERRIDE
        {
            uiWebWrapTimer      = 11*IN_MILLISECONDS;
            uiInfectedBiteTimer = 4*IN_MILLISECONDS;
            uiBindingWebsTimer = 17*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiWebWrapTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_WEB_WRAP, true);
                uiWebWrapTimer = 15*IN_MILLISECONDS;
            } else uiWebWrapTimer -= diff;

            if (uiInfectedBiteTimer <= diff)
            {
                DoCastVictim(SPELL_INFECTED_BITE, true);
                uiInfectedBiteTimer = 11*IN_MILLISECONDS;
            } else uiInfectedBiteTimer -= diff;

            if (uiBindingWebsTimer <= diff)
            {
                DoCastVictim(SPELL_BLINDING_WEBS, true);
                uiBindingWebsTimer = 17*IN_MILLISECONDS;
            } else uiBindingWebsTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_watcher_narjilAI(creature);
    }
};

class npc_watcher_silthik : public CreatureScript
{
public:
    npc_watcher_silthik() : CreatureScript("npc_watcher_silthik") { }

    struct npc_watcher_silthikAI : public ScriptedAI
    {
        npc_watcher_silthikAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiWebWrapTimer;
        uint32 uiInfectedBiteTimer;
        uint32 uiPoisonSprayTimer;

        void Reset() OVERRIDE
        {
            uiWebWrapTimer      = 11*IN_MILLISECONDS;
            uiInfectedBiteTimer = 4*IN_MILLISECONDS;
            uiPoisonSprayTimer  = 15*IN_MILLISECONDS;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (uiWebWrapTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_WEB_WRAP, true);

                uiWebWrapTimer = 15*IN_MILLISECONDS;
            } else uiWebWrapTimer -= diff;

            if (uiInfectedBiteTimer <= diff)
            {
                DoCastVictim(SPELL_INFECTED_BITE, true);
                uiInfectedBiteTimer = 15*IN_MILLISECONDS;
            } else uiInfectedBiteTimer -= diff;

            if (uiPoisonSprayTimer <= diff)
            {
                DoCastVictim(SPELL_POSION_SPRAY, true);
                uiPoisonSprayTimer = 17*IN_MILLISECONDS;
            } else uiPoisonSprayTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_watcher_silthikAI(creature);
    }
};

class achievement_watch_him_die : public AchievementCriteriaScript
{
    public:
        achievement_watch_him_die() : AchievementCriteriaScript("achievement_watch_him_die")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            InstanceScript* instance = target->GetInstanceScript();
            if (!instance)
                return false;

            for (uint8 n = 0; n < 3; ++n)
            {
                if (Creature* watcher = ObjectAccessor::GetCreature(*target, instance->GetData64(DATA_WATCHER_GASHRA + n)))
                    if (!watcher->IsAlive())
                        return false;
            }

            return true;
        }
};

void AddSC_boss_krik_thir()
{
    new boss_krik_thir();
    new npc_skittering_infector();
    new npc_anub_ar_skirmisher();
    new npc_anub_ar_shadowcaster();
    new npc_watcher_gashra();
    new npc_anub_ar_warrior();
    new npc_watcher_silthik();
    new npc_watcher_narjil();
    new achievement_watch_him_die();
}
