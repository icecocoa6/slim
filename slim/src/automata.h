/*
 * automata.h
 *
 *   Copyright (c) 2008, Ueda Laboratory LMNtal Group
 *                                         <lmntal@ueda.info.waseda.ac.jp>
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are
 *   met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *    3. Neither the name of the Ueda Laboratory LMNtal Group nor the
 *       names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */

#ifndef LMN_NEVER_CLAIM
#define LMN_NEVER_CLAIM

#include <stdio.h>
#include "lmntal.h"
#include "vector.h"

typedef struct Automata *Automata;
typedef struct AutomataState *AutomataState;
typedef struct Transition *Transition;

typedef BYTE atmstate_id_t;

/* Propositional Logic Formula */
typedef struct PLFormula *PLFormula;


/* automata */

Automata automata_make(void);
void automata_free(Automata a);
unsigned int automata_state_id(Automata a, char *state_name);
const char *automata_state_name(Automata a, atmstate_id_t id);
AutomataState automata_get_state(Automata a, BYTE state_id);
void automata_set_init_state(Automata a, atmstate_id_t id);
atmstate_id_t automata_get_init_state(Automata a);
unsigned int automata_propsym_to_id(Automata a, char *prop_name);
AutomataState atmstate_make(unsigned int id,
                            BOOL is_accept_state,
                            BOOL is_end_state);

/* state of automata */

void atmstate_add_transition(AutomataState s, Transition t);
void automata_add_state(Automata a, AutomataState s);
atmstate_id_t atmstate_id(AutomataState s);
unsigned int atmstate_transition_num(AutomataState s);
Transition atmstate_get_transition(AutomataState s, unsigned int index);
BOOL atmstate_is_accept(AutomataState s);
BOOL atmstate_is_end(AutomataState s);

/* transition of automata */

Transition transition_make(unsigned int next, PLFormula f);
BYTE transition_next(Transition t);
PLFormula transition_get_formula(Transition t);

/* propositional Logic Formula */

PLFormula true_node_make(void);
PLFormula false_node_make(void);
PLFormula sym_node_make(int sym_id);
PLFormula negation_node_make(PLFormula f0);
PLFormula and_node_make(PLFormula f0, PLFormula f1);
PLFormula or_node_make(PLFormula f0, PLFormula f1);
void free_formula(PLFormula f);
BOOL eval_formula(LmnMembrane *mem, Vector *prop_defs, PLFormula f);

/* never claim */

int never_claim_load(FILE *f, Automata *a);

#endif
