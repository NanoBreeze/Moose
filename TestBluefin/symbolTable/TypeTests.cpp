#include "pch.h"

#include "antlr4-runtime.h"
#include "../utils.h"
#include <iostream>
#include <fstream>
#include <map>

#include "../../symbolTable/SymbolTable.h"
#include "../../symbolTable/SymbolFactory.h"
#include "../../listeners/Declaration.h"
#include "../../listeners/Resolution.h"
#include "../../listeners/DecorateExprWithTypes.h"

#include "PostOrderPrintType.h"
#include "SymbolTableTestWrapper.h"
#include "SymbolWrapperFactory.h"

namespace SymbolTableTests {

	using std::string;
	using std::ifstream;
	using antlr4::tree::ParseTree;
	using antlr4::tree::ParseTreeWalker;
	using std::map;
	using namespace bluefin;
	using namespace antlr4;


	void validateTypeChecking(const string programFile, const string expectedOutputFile) {
		string pathPrefix = "../TestBluefin/symbolTable/programs/TypeChecking/";

		ifstream file(pathPrefix + programFile);
		ParseTree* tree = createParseTree(file);

		ParseTreeWalker walker;

		SymbolTable symTab;
		SymbolFactory symFact;

		Declaration declarationListener(symTab, symFact);
		walker.walk(&declarationListener, tree);

		Resolution resolutionListener(declarationListener.getScopes(), symTab);
		walker.walk(&resolutionListener, tree);

		map<ParseTree*, shared_ptr<Scope>> scopes = declarationListener.getScopes(); 
		DecorateExprWithTypes decorateExprListener(scopes, symFact, symTab);
		walker.walk(&decorateExprListener, tree);

		string symbolTypesStr;
		PostOrderPrintType printTypeListener(symbolTypesStr, decorateExprListener.getTypeContexts());
		walker.walk(&printTypeListener, tree);

		string expectedOutput = readFile(pathPrefix + expectedOutputFile);
		ASSERT_EQ(symbolTypesStr, expectedOutput);
	}

	TEST(Types, Program_ExprTypesForPrimaries) {
		validateTypeChecking("ExprTypesForPrimaries.bf", "ExprTypesForPrimaries_expected.txt");
	}

	TEST(Types, Program_ExprTypesForRelationalOps) {
		validateTypeChecking("ExprTypesForRelationalOps.bf", "ExprTypesForRelationalOps_expected.txt");
	}

	TEST(Types, Program_ExprTypesForEqualityOps) {
		validateTypeChecking("ExprTypesForEqualityOps.bf", "ExprTypesForEqualityOps_expected.txt");
	}

	TEST(Types, Program_ExprTypesForArithmeticBinaryOps) {
		validateTypeChecking("ExprTypesForArithmeticBinaryOps.bf", "ExprTypesForArithmeticBinaryOps_expected.txt");
	}

	TEST(Types, Program_ExprTypesForUnaryOps) {
		validateTypeChecking("ExprTypesForUnaryOp.bf", "ExprTypesForUnaryOp_expected.txt");
	}

	TEST(Types, Program_ExprTypesForLogicalAndOr) {
		validateTypeChecking("ExprTypesForLogicalAndOr.bf", "ExprTypesForLogicalAndOr_expected.txt");
	}

	TEST(Types, Program_ExprTypesForFuncCalls) {
		validateTypeChecking("ExprTypesForFuncCalls.bf", "ExprTypesForFuncCalls_expected.txt");
	}

	TEST(Types, Program_ExprTypesForStructMembers) {
		validateTypeChecking("ExprTypesForStructMembers.bf", "ExprTypesForStructMembers_expected.txt");
	}

	TEST(Types, Program_ExprTypesForAssignments) {
		validateTypeChecking("ExprTypesForAssignments.bf", "ExprTypesForAssignments_expected.txt");
	}

	TEST(Types, Program_PromotionTypeForVarDecl) {
		validateTypeChecking("PromotionTypeForVarDecl.bf", "PromotionTypeForVarDecl_expected.txt");
	}

	TEST(Types, Program_PromotionTypeForFuncCallArgs) {
		validateTypeChecking("PromotionTypeForFuncCallArgs.bf", "PromotionTypeForFuncCallArgs_expected.txt");
	}

	TEST(Types, Program_ExprTypesForReturnStmt) {
		validateTypeChecking("ExprTypesForReturnStmt.bf", "ExprTypesForReturnStmt_expected.txt");
	}

}
