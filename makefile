CC=g++
CFLAGS= -std=c++11

Lisp: Lisp.cpp Expr.o Parser.o Lexer.o
	$(CC) -o Lisp Lisp.cpp Expr.o Parser.o Lexer.o $(CFLAGS) -I.

Parser.o: Parser.cpp Parser.cpp Lexer.h Expr.h
	$(CC) -c Parser.cpp $(CFLAGS)

Lexer.o: Lexer.cpp Lexer.h Expr.h
	$(CC) -c Lexer.cpp $(CFLAGS)

Expr.o: Expr.cpp Expr.h
	$(CC) -c Expr.cpp $(CFLAGS)
