/**
 * This file includes all definitions necessary to interact with Trail's voting system. Developers who want to
 * utilize the system simply must include this file in their implementation to interact with the information
 * stored by Trail.
 * 
 * @author Craig Branscom
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/permission.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/singleton.hpp>

using namespace std;
using namespace eosio;

#pragma region Constants

uint64_t const VOTE_ISSUE_RATIO = 1; //indicates a 1:1 TLOS/VOTE issuance

#pragma endregion Constants

#pragma region Structs

///@abi table votereceipts i64
struct vote_receipt {
    uint64_t receipt_id;
    uint64_t ballot_id;
    uint16_t direction;
    asset weight;
    uint32_t expiration;

    uint64_t primary_key() const { return receipt_id; }
    EOSLIB_SERIALIZE(vote_receipt, (receipt_id)(ballot_id)(voter)(direction)(weight)(expiration))
};

/// @abi table voters i64
struct voter_id {
    account_name voter;

    asset liquid_votes;
    asset spent_votes;

    uint32_t release_time;

    uint64_t primary_key() const { return voter; }
    EOSLIB_SERIALIZE(voter_id, (voter)
        (staked_tlos)(liquid_votes)(spent_votes))
};

/// @abi table ballots
struct ballot {
    uint64_t ballot_id;
    account_name publisher;
    
    asset no_count;
    asset yes_count;
    asset abstain_count;
    uint32_t unique_voters;

    uint32_t begin_time;
    uint32_t end_time;
    bool status; // 0 = FAIL, 1 = PASS

    uint64_t primary_key() const { return ballot_id; }
    EOSLIB_SERIALIZE(ballot, (ballot_id)(publisher)
        (no_count)(yes_count)(abstain_count)(unique_voters)
        (begin_time)(end_time)(status))
};

#pragma endregion Structs

#pragma region Tables

typedef multi_index<N(voters), voter_id> voters_table;

typedef multi_index<N(ballots), ballot> ballots_table;

//typedef multi_index<N(votestakes), vote_receipt> votereceipts_table;

#pragma endregion Tables

#pragma region Helper_Functions

bool is_voter(account_name voter) {
    voters_table voters(N(eosio.trail), N(eosio.trail));
    auto v = voters.find(voter);

    if (v != voters.end()) {
        return true;
    }

    return false;
}

bool is_ballot(uint64_t ballot_id) {
    ballots_table ballots(N(eosio.trail), N(eosio.trail));
    auto b = ballots.find(ballot_id);

    if (b != ballots.end()) {
        return true;
    }

    return false;
}

symbol_name get_ballot_sym(uint64_t ballot_id) {
    ballots_table ballots(N(eosio.trail), N(eosio.trail));
    auto b = ballots.get(ballot_id);

    return b.no_count.symbol.name();
}

#pragma endregion Helper_Functions