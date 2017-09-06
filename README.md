# Well-Formed-Formula-Checker
A lightweight C program that checks if a Statement Logic formula is well formed as defined by my
Logic Professor, Professor Alistair Welchmn, and in The Power Of Logic by Frances Howard-Snyder,
Daniel Howard-Snyder, and Ryan Wasserman.

Well formed formulas are defined by a few rules.
1. Outside parenthesis or brackets are neccessary unless the first character is a negation operator.
2. Operators within the formula are either unary(negation) or binary(all others). A binary operator
    may only be present once in each level of nested parenthesis or brackets. (aka (A.B.C) is not a
    WFF whereas ((A.B).C) or (A.(B.C)) is, even though they all semantically mean the same thing)
3. An atomic statement is always a well formed formula.
4. An atmoic statement must be represented by a single capital letter.
5. A compound statement is a well formed formula if its substatements are also well formed formulas.

The WFF checker will test a formula to see if it fits these rules. It does this by taking the formula
and saving its substatements in a binary tree by splitting the full statement on its main logical
operator. If all the leaves of the tree are capital letters, then the formula is well formed! Example
output is included in results.txt

Additional note: Xor is not supported in The Power of Logic, and can be substituted by
((AvB).~(A.B)) === A or B but not both.
