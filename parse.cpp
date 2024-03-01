/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
 */

#include "parse.h"

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser
{
    bool pushed_back = false;
    LexItem pushed_token;

    static LexItem GetNextToken(istream &in, int &line)
    {
        if (pushed_back)
        {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem &t)
    {
        if (pushed_back)
        {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}

// Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
// Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt
bool Stmt(istream &in, int &line)
{
    bool status;
    // cout << "in ContrlStmt" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    switch (t.GetToken())
    {

    case WRITELN:
        status = WriteLnStmt(in, line);
        // cout << "After WriteStmet status: " << (status? true:false) <<endl;
        break;

    case IF:
        status = IfStmt(in, line);
        break;

    case IDENT:
        Parser::PushBackToken(t);
        status = AssignStmt(in, line);

        break;

    case FOR:
        status = ForStmt(in, line);

        break;

    default:
        Parser::PushBackToken(t);
        return false;
    }

    return status;
} // End of Stmt

// WriteStmt:= wi, ExpreList
bool WriteLnStmt(istream &in, int &line)
{
    LexItem t;
    // cout << "in WriteStmt" << endl;

    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {

        ParseError(line, "Missing Left Parenthesis");
        return false;
    }

    bool ex = ExprList(in, line);

    if (!ex)
    {
        ParseError(line, "Missing expression after WriteLn");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {

        ParseError(line, "Missing Right Parenthesis");
        return false;
    }
    // Evaluate: print out the list of expressions values

    return ex;
}
bool Prog(istream &in, int &line)
{
    return false;
}

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
    bool status = false;
    // cout << "in ExprList and before calling Expr" << endl;
    status = Expr(in, line);
    if (!status)
    {
        ParseError(line, "Missing Expression");
        return false;
    }

    LexItem tok = Parser::GetNextToken(in, line);

    if (tok == COMMA)
    {
        // cout << "before calling ExprList" << endl;
        status = ExprList(in, line);
        // cout << "after calling ExprList" << endl;
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}
bool IfStmt(istream &in, int &line)
{
    return false;
}
bool AssignStmt(istream &in, int &line)
{
    bool varstatus = false;
    bool status = false;
    LexItem t;
    varstatus = Var(in, line);

    if (varstatus)
    {
        LexItem t = Parser::GetNextToken(in, line);
        if (t == ASSOP)
        {
            status = Expr(in, line);
            if (!status)
            {
                ParseError(line, "Missing Expression in Assignment Statement");
                return status;
            }
        }
        else if (t.GetToken() == ERR)
        {
            ParseError(line, "Unregognized Input Pattern");
            return false;
        }
        else
        {
            ParseError(line, "Missing Assignment Operator :=");
            return false;
        }
    }
    return false;
}
bool ForStmt(istream &in, int &line)
{
    return false;
}
bool Expr(istream &in, int &line)
{
    bool t1 = Term(in, line);
    LexItem tok = Parser ::GetNextToken(in, line);
    if (!t1)
    {
        return false;
    }
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        return false;
    }
    while (tok == PLUS || tok == MINUS)
    {
        t1 = Term(in, line);
        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
}
bool Term(istream &in, int &line)
{
    bool status = SFactor(in, line);
    LexItem t = Parser::GetNextToken(in, line);

    if (!status)
    {
        return false;
    }

    if (t.GetToken() == ERR)
    {
        ParseError(line, "Missing operand after operator");
        return false;
    }
    while (t == MULT || t == DIV)
    {
        status = SFactor(in, line);
        if (!status)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(t);
    return true;
}
bool SFactor(istream &in, int &line)
{
    return false;
}
bool Var(istream &in, int &line)
{
    return false;
}