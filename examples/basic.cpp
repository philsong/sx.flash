#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

#include <flash.sx.hpp>
#include <eosio.token.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]] basic : public contract {

public:
	using contract::contract;

	[[eosio::action]]
	void init( const asset quantity )
	{
		flash::borrow_action borrow( "flash.sx"_n, { get_self(), "active"_n });
		borrow.send( get_self(), "eosio.token"_n, quantity, "", set<name>() );
	}

	[[eosio::on_notify("eosio.token::transfer")]]
	void on_transfer( const name from, const name to, asset quantity, const string memo )
	{
		// only handle incoming transfers from contract
		if ( from != "flash.sx"_n ) return;

		// do actions before sending funds back
		// PLACE YOUR CODE HERE

		// repay flashloan
		token::transfer_action transfer( "eosio.token"_n, { get_self(), "active"_n });
		transfer.send( get_self(), from, quantity, memo );
	}
};
