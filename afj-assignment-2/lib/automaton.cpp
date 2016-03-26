//
//  automaton.cpp
//  afj-assignment-2
//
//  Created by Martin Kiesel on 24/03/16.
//  Copyright © 2016 Martin Kiesel. All rights reserved.
//

#include "automaton.hpp"

#include <iostream>

bool Automaton::accepts(string word)
{
    if (initial_state == -1 || dfa == false) return false;
    int current_state = initial_state;
    int transitions_traversed = 0;

    for (auto &ch : word)
    {
        string character = string(1, ch);
        if (alphabet.find(character) == alphabet.end()) return false;

        for (const auto &transition : transitions)
        {
            if (transition.from == current_state && transition.input == character)
            {
                current_state = transition.to;
                transitions_traversed++;
                break;
            }
        }
    }

    if (transitions_traversed == word.length() && states.at(current_state).final)
        return true;
    return false;
}

void Automaton::nka2dka()
{
    map<string, vstate> nstates;
    vstate init = eClosure(states[initial_state]);
    nstates[groupStateName(init)] = init;

    
}

string Automaton::groupStateName(vstate vs)
{
    bool first = true;
    string name = "";

    for (const auto &s : vs)
    {
        if (!first) name += ", ";
        else first = false;
        name += s.name;
    }
    return name;
}

vstate Automaton::eClosure(state s)
{
    vstate rstates;
    rstates.push_back(s);

    for (const auto &t : transitions)
    {
        if (t.epsilon && s.id == t.from && !existsInVState(rstates, t.to))
        {
            vstate tmp = eClosure(states[t.to]);
            rstates.insert(rstates.end(), tmp.begin(), tmp.end());
        }
    }
    sort(rstates.begin(), rstates.end());
    return rstates;
}

bool Automaton::existsInVState(vstate vs, int id)
{
    for (const auto &s : vs)
    {
        if (s.id == id) return true;
    }
    return  false;
}

bool Automaton::existsInVState(vstate vs, state s)
{
    return existsInVState(vs, s.id);
}

void Automaton::determineType()
{
    dfa = true;

    map<int, int> state_transition;

    for (const auto &t : transitions)
    {
        if (t.input == EPSILON_STRING) //t.from != initial_state && 
        {
            dfa = false;
            return;
        }

        if (!state_transition.count(t.from))
            state_transition[t.from] = 0;
        state_transition.at(t.from)++;
    }

    size_t alphabet_size = alphabet.size();

    for (const auto &st : state_transition)
    {
        if (st.second > alphabet_size)
        {
            dfa = false;
            return;
        }
    }

    for (const auto &st : states)
    {
        const int id = st.second.id;
        map<string, bool> used_alpha;

        for (const auto &t : transitions)
        {
            if (t.from != id) continue;
            if (used_alpha.count(t.input))
            {
                dfa = false;
                return;
            }
            used_alpha[t.input] = true;
        }
    }
}