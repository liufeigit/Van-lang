/***
	1. van 类bnf部分语法

	Program=States;

	State=Expr State
		  | Assign State
		  | If State
		  | While State
		  | For State
		  | Fundef States
		  | Up State
		  | Continue State
		  | Break State
		  | Return State
		  | Classdef States
		  | Field State
		  | Print State
		  ;

	Expr=Identifi
		| out Identifi
		| Number
		| Null
		| True
		| False
		| Self
		| Super
		| String

		| Map
		| List

		| FuncExpr 
		| ClassExpr

		| (Expr)

		| !Expr
		| -Expr
		| ~Expr
		| new Expr

		| Expr(ArgumentsExpr)
		| Expr.Identifi
		| Expr[Expr]
		| Expr++
		| Expr--

		| Expr+Expr
		| Expr-Expr
		| Expr*Expr
		| Expr/Expr
		| Expr%Expr

		| Expr>Expr
		| Expr>=Expr
		| Expr<Expr
		| Expr<=Expr
		| Expr==Expr
		| Expr!=Expr

		| Expr&&Expr
		| Expr||Expr
		
		| Expr<<Expr
		| Expr>>Expr
		| Expr&Expr
		| Expr|Expr

		| Expr?Expr:Expr
		| Expr,Expr
		;
		
	Identifi="[_a-zA-Z][_a-zA-Z0-9]*";

	Lital=Number
		 |String
		 |true
		 |false
		 |null
		 |Array
		 |Object
		 |FunctionExpr
		 |ClassExpr
		 ;

	Number=[-{0}.][1-9][0-9]*;
	String=" " " .* " " ";

	Array="["
		Expr,Expr,...
	"]";
	Object={
		Identifi : Expr,...
	};

	FunctionExpr=def (ArgumentsDef){ State }
	ClassExpr=class (Expr){ [Field|Fundef] }
	
	LeftExpr=Identifi|Expr.Identifi|Expr[Expr];
	Assign=LeftExpr,LeftExpr,...=Expr,Expr,...;

	If= if ( Expr ){ States } [else if( Expr ){ States }] [ else{ States } ];
	While= while( Expr ){ State };
	For= for ( Identifi in Expr  ){ States };
	Fundef=def Identifi( ArgumentsDef ){ States };
	Classdef= class Identifi(Expr){ [Field|Fundef] };

	Up= up [Identifi]*;
	Field= field [Identifi]*;
	Print= print [Expr]*;
	
	ArgumentsExpr=[Expr]*;
	ArgumentsDef=[[out] Identifi]*;

****/








