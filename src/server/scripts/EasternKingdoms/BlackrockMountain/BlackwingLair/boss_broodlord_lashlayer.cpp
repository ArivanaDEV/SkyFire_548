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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackwing_lair.h"

enum Say
{
    SAY_AGGRO               = 0,
    SAY_LEASH               = 1
};

enum Spells
{
    SPELL_CLEAVE            = 26350,
    SPELL_BLASTWAVE         = 23331,
    SPELL_MORTALSTRIKE      = 24573,
    SPELL_KNOCKBACK         = 25778
};

enum Events
{
    EVENT_CLEAVE            = 1,
    EVENT_BLASTWAVE         = 2,
    EVENT_MORTALSTRIKE      = 3,
    EVENT_KNOCKBACK         = 4,
    EVENT_CHECK             = 5
};

class boss_broodlord : public CreatureScript
{
public:
    boss_broodlord() : CreatureScript("boss_broodlord") { }

    struct boss_broodlordAI : public BossAI
    {
        boss_broodlordAI(Creature* creature) : BossAI(creature, BOSS_BROODLORD) { }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            if (instance->GetBossState(BOSS_VAELASTRAZ) != DONE)
            {
                EnterEvadeMode();
                return;
            }

            _EnterCombat();
            Talk(SAY_AGGRO);

            events.ScheduleEvent(EVENT_CLEAVE, 8000);
            events.ScheduleEvent(EVENT_BLASTWAVE, 12000);
            events.ScheduleEvent(EVENT_MORTALSTRIKE, 20000);
            events.ScheduleEvent(EVENT_KNOCKBACK, 30000);
            events.ScheduleEvent(EVENT_CHECK, 1000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 7000);
                        break;
                    case EVENT_BLASTWAVE:
                        DoCastVictim(SPELL_BLASTWAVE);
                        events.ScheduleEvent(EVENT_BLASTWAVE, urand(8000, 16000));
                        break;
                    case EVENT_MORTALSTRIKE:
                        DoCastVictim(SPELL_MORTALSTRIKE);
                        events.ScheduleEvent(EVENT_MORTALSTRIKE, urand(25000, 35000));
                        break;
                    case EVENT_KNOCKBACK:
                        DoCastVictim(SPELL_KNOCKBACK);
                        if (DoGetThreat(me->GetVictim()))
                            DoModifyThreatPercent(me->GetVictim(), -50);
                        events.ScheduleEvent(EVENT_KNOCKBACK, urand(15000, 30000));
                        break;
                    case EVENT_CHECK:
                        if (me->GetDistance(me->GetHomePosition()) > 150.0f)
                        {
                            Talk(SAY_LEASH);
                            EnterEvadeMode();
                        }
                        events.ScheduleEvent(EVENT_CHECK, 1000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_broodlordAI(creature);
    }
};

void AddSC_boss_broodlord()
{
    new boss_broodlord();
}
