/**
 * 
 * 
 * @author Craig Branscom
 */

#include <trail.defs/trail.voting.hpp>
#include <trail.defs/trail.system.hpp>
#include <trail.defs/trail.tokens.hpp>

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <string>

using namespace eosio;

class trail : public contract {
    
    public:

        trail(account_name self);

        ~trail();

        #pragma region Constants

        uint64_t const VOTE_ISSUE_RATIO = 1; //indicates a 1:1 TLOS/VOTE issuance

        uint32_t const MIN_LOCK_PERIOD = 86400; //86,400 seconds is ~1 day

        uint32_t const MAX_LOCK_PERIOD = 7776000; //7,776,000 seconds is ~3 months

        #pragma endregion Constants

        #pragma region Token_Actions

        /// @abi action
        void regtoken(asset native, account_name publisher);

        /// @abi action
        void unregtoken(asset native, account_name publisher);

        #pragma endregion Token_Actions

        #pragma region Voting_Actions

        /// @abi action
        void regvoter(account_name voter);

        /// @abi action
        void unregvoter(account_name voter);

        /// @abi action
        void regballot(account_name publisher, asset voting_token, uint32_t begin_time, uint32_t end_time, string info_url);

        /// @abi action
        void unregballot(account_name publisher, uint64_t ballot_id);

        /// @abi action
        void getvotes(account_name voter, uint32_t lock_period);

        /// @abi action
        void castvote(account_name voter, uint64_t ballot_id, uint16_t direction);

        /// @abi action
        void nextcycle(account_name publisher, uint64_t ballot_id, uint32_t new_begin_time, uint32_t new_end_time);

        /// @abi action
        void deloldvotes(account_name voter, uint16_t num_to_delete);

        #pragma endregion Voting_Actions

        #pragma region Reactions

        //Reactions are regular functions called only as a trigger from the dispatcher.

        void closevote(account_name publisher, uint64_t ballot_id, bool pass);

        #pragma endregion Reactions

    protected:

        /// @abi table environment i64
        struct env {
            account_name publisher;
            
            uint64_t total_tokens;
            uint64_t total_voters;
            uint64_t total_ballots;

            asset vote_supply;

            uint32_t time_now;

            uint64_t primary_key() const { return publisher; }
            EOSLIB_SERIALIZE(env, (publisher)
                (total_tokens)(total_voters)(total_ballots)
                (vote_supply)
                (time_now))
        };

        typedef singleton<N(environment), env> environment_singleton;
        environment_singleton environment;
        env env_struct;
};
