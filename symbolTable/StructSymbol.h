#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include "Symbol.h"
#include "Scope.h"

namespace bluefin {

	using std::string;
	using std::shared_ptr;
	using std::vector;

	/** 
	Might be circular reference to have a sp to enclosing scope. Consider using wp
	*/
	class StructSymbol : public Symbol, public Scope
	{

	public:
		StructSymbol(const string& name, shared_ptr<Scope> enclosingScope, size_t tokenIndex, shared_ptr<StructSymbol> superClass=nullptr) : 
			Symbol(name, Type{ name }, tokenIndex), Scope{ enclosingScope, name }, superClass{ superClass }
		{}

		shared_ptr<Scope> getParentScope() const override;

		inline shared_ptr<StructSymbol> getSuperClass() const { return superClass; }

		using Symbol::getName; // both Scope and Symbol have "getName()". If user calls StructSymbol::getName()
		// refer to the Symbol's

		size_t getMemberIndex(string memName) const;

		void declare(shared_ptr<Symbol> symbol) override;

	private:
		shared_ptr<StructSymbol> superClass;
		vector<string> memNames;
	};

}
