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

/* ScriptData
SDName: Boss_Harbinger_Skyriss
SD%Complete: 45
SDComment: CombatAI not fully implemented. Timers will need adjustments. Need more docs on how event fully work. Reset all event and force start over if fail at one point?
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

/* ContentData
boss_harbinger_skyriss
boss_harbinger_skyriss_illusion
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "arcatraz.h"

enum Says
{
    SAY_INTRO              = 0,
    SAY_AGGRO              = 1,
    SAY_KILL               = 2,
    SAY_MIND               = 3,
    SAY_FEAR               = 4,
    SAY_IMAGE              = 5,
    SAY_DEATH              = 6
};

enum Spells
{
    SPELL_FEAR              = 39415,
    SPELL_MIND_REND         = 36924,
    H_SPELL_MIND_REND       = 39017,
    SPELL_DOMINATION        = 37162,
    H_SPELL_DOMINATION      = 39019,
    H_SPELL_MANA_BURN       = 39020,
    SPELL_66_ILLUSION       = 36931,                       //entry 21466
    SPELL_33_ILLUSION       = 36932,                       //entry 21467

    SPELL_MIND_REND_IMAGE   = 36929,
    H_SPELL_MIND_REND_IMAGE = 39021
};

class boss_harbinger_skyriss : public CreatureScript
{
    public:
        boss_harbinger_skyriss() : CreatureScript("boss_harbinger_skyriss") { }

        struct boss_harbinger_skyrissAI : public ScriptedAI
        {
            boss_harbinger_skyrissAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                Intro = false;
            }

            InstanceScript* instance;

            bool Intro;
            bool IsImage33;
            bool IsImage66;

            uint32 Intro_Phase;
            uint32 Intro_Timer;
            uint32 MindRend_Timer;
            uint32 Fear_Timer;
            uint32 Domination_Timer;
            uint32 ManaBurn_Timer;

            void Reset() OVERRIDE
            {
                if (!Intro)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                IsImage33 = false;
                IsImage66 = false;

                Intro_Phase = 1;
                Intro_Timer = 5000;
                MindRend_Timer = 3000;
                Fear_Timer = 15000;
                Domination_Timer = 30000;
                ManaBurn_Timer = 25000;
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE

            {
                if (!Intro)
                    return;

                ScriptedAI::MoveInLineOfSight(who);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE { }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DEATH);
                if (instance)
                    instance->SetData(TYPE_HARBINGERSKYRISS, DONE);
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                if (!summon)
                    return;
                if (IsImage66)
                    summon->SetHealth(summon->CountPctFromMaxHealth(33));
                else
                    summon->SetHealth(summon->CountPctFromMaxHealth(66));
                if (me->GetVictim())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        summon->AI()->AttackStart(target);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                //won't yell killing pet/other unit
                if (victim->GetEntry() == 21436)
                    return;

                Talk(SAY_KILL);
            }

            void DoSplit(uint32 val)
            {
                if (me->IsNonMeleeSpellCasted(false))
                    me->InterruptNonMeleeSpells(false);

                Talk(SAY_IMAGE);

                if (val == 66)
                    DoCast(me, SPELL_66_ILLUSION);
                else
                    DoCast(me, SPELL_33_ILLUSION);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!Intro)
                {
                    if (!instance)
                        return;

                    if (Intro_Timer <= diff)
                    {
                        switch (Intro_Phase)
                        {
                        case 1:
                            Talk(SAY_INTRO);
                            instance->HandleGameObject(instance->GetData64(DATA_SPHERE_SHIELD), true);
                            ++Intro_Phase;
                            Intro_Timer = 25000;
                            break;
                        case 2:
                            Talk(SAY_AGGRO);
                            if (Unit* mellic = Unit::GetUnit(*me, instance->GetData64(DATA_MELLICHAR)))
                            {
                                //should have a better way to do this. possibly spell exist.
                                mellic->setDeathState(DeathState::JUST_DIED);
                                mellic->SetHealth(0);
                                instance->SetData(TYPE_SHIELD_OPEN, IN_PROGRESS);
                            }
                            ++Intro_Phase;
                            Intro_Timer = 3000;
                            break;
                        case 3:
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                            Intro = true;
                            break;
                        }
                    }
                    else
                        Intro_Timer -=diff;
                }
                if (!UpdateVictim())
                    return;

                if (!IsImage66 && !HealthAbovePct(66))
                {
                    DoSplit(66);
                    IsImage66 = true;
                }
                if (!IsImage33 && !HealthAbovePct(33))
                {
                    DoSplit(33);
                    IsImage33 = true;
                }

                if (MindRend_Timer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_MIND_REND);
                    else
                        DoCastVictim(SPELL_MIND_REND);

                    MindRend_Timer = 8000;
                }
                else
                    MindRend_Timer -=diff;

                if (Fear_Timer <= diff)
                {
                    if (me->IsNonMeleeSpellCasted(false))
                        return;

                    Talk(SAY_FEAR);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_FEAR);
                    else
                        DoCastVictim(SPELL_FEAR);

                    Fear_Timer = 25000;
                }
                else
                    Fear_Timer -=diff;

                if (Domination_Timer <= diff)
                {
                    if (me->IsNonMeleeSpellCasted(false))
                        return;

                    Talk(SAY_MIND);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        DoCast(target, SPELL_DOMINATION);
                    else
                        DoCastVictim(SPELL_DOMINATION);

                    Domination_Timer = 16000+rand()%16000;
                }
                else
                    Domination_Timer -=diff;

                if (IsHeroic())
                {
                    if (ManaBurn_Timer <= diff)
                    {
                        if (me->IsNonMeleeSpellCasted(false))
                            return;

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                            DoCast(target, H_SPELL_MANA_BURN);

                        ManaBurn_Timer = 16000+rand()%16000;
                    }
                    else
                        ManaBurn_Timer -=diff;
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_harbinger_skyrissAI(creature);
        }
};

class boss_harbinger_skyriss_illusion : public CreatureScript
{
    public:
        boss_harbinger_skyriss_illusion() : CreatureScript("boss_harbinger_skyriss_illusion") { }

        struct boss_harbinger_skyriss_illusionAI : public ScriptedAI
        {
            boss_harbinger_skyriss_illusionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE { }

            void EnterCombat(Unit* /*who*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_harbinger_skyriss_illusionAI(creature);
        }
};

void AddSC_boss_harbinger_skyriss()
{
    new boss_harbinger_skyriss();
    new boss_harbinger_skyriss_illusion();
}

