
## July 6:

started writing machine code. thinking i'm not going to do the traditional low-level stack-based machine, but instead try my luck at implementing something more macro-instruction-ish. i suspect this isn't the best idea, but i want to feel the pain and understand why.

i'm thinking the language will be something like javascript, but cleaned up a bit.  higher order functions, but classes and inheritance instead of objects, and no prototypal inheritance.

One number type, 80bit long doubles -- that gives 64 bits of integer precision, but is a waste of space. i'm not super upset about that right now. all strings are utf-8, entire system will be utf-8 safe.

will code this in C++ -- i have not written C++ in a long time, going to be fun learning all this again.


## July 7: 

continued coding machine instructions, started implementing various pieces.

turns out that doing UTF-8 in c++ is a bit of a hassle. playing around with a few solutions.  feels like PHP circa 2004. i know that there some utf-8 compatible classes out there (glibc in particular), but trying to keep this as generic as possible.  had considered boost, but it looks pretty heavyweight.


## July 10: 

i'm basically imagining what the compiler will generate, and then writing the VM for that.  

Coding up some instruction parsing. going pretty well. have support for labels and starting to add a few basic instructions. i'm going to start with the the following programs:

```
var x = 5 + 2;

ADDVAR     "x", unknown   ; declare a new variable
ADD        5, 2, "x"
DEBUGPRINT "x"            ; special debugging tool
EXIT       0


var x = (5 * 3) + (4 / 3);

ADDVAR     "x", unknown
GETTEMP    TMP1, number    ; allocate register-like storage
MUL        5, 3, TMP1
GETTEMP    TMP2, number
DIV        4, 3, TMP2
ADD        TMP1, TMP2, 'x"
DEBUGPRINT "x"             ; special debugging tool
EXIT       0
```

## July 15:

Instruction parsing and argument lists parsed fully.  Starting to add infrastructure for variable pools and instruction types.


## July 20:

Starting to add supports for variables -- first type: numbers.

Concern: Winders doesn't have long double.  Argh.


## July 22:

Started to code up the execute functions. I'm going to need stacks of var pools / scopes.


## July 23:

Coded up Scope Stacks. I think refcounting and copy constructors are going to solve all of my problems, including the crucial:

```
var x = 6;

function yy() {

    var y = 10;
    setTimeout(function () {
        var z = 15;
        console.log(x + " " +y+ " " + z);
    }, 3000);

    y = 12;
    function zz() {
        y = 13;
    }

    zz();
    console.log(x + " " +y);
}

yy();
```

Should print: 

```
6 13
6 13 15
```


    

## July 24:

So, math operations are working now. I've moved the implementation of all math operations into a single "mathops" class, which should be fine.

I also decided the way I was doing temp vars was stupid.  i'll have the compiler worry about allocating and freeing temp vars, since it can also choose unique names and knows how to construct blocks where it deletes stuff.  i'll also give temp vars (sort of like registers, I guess) have names that start with chars (::) that aren't allowed anywhere else in the language.

I've now got a bunch of test programs that work great.  Super excited!!!

## July 28: 

Compare instruction done. I now have a set of "flags" that reflect result of latest COMPARE instruction.

Possible future optimisation: Like a real CPU, allow other operations to set flags and save the COMPARE instruction.

## July 29:

I don't like the way i'm executing things -- scope stack is doing too much, creating an execution engine and instruction runners so that i can do things like run multiple modules, functions, branching, etc.


## July 30:

HUGE MILESTONE: branching instrutions done. I can now run factorial as a loop.

Next step: functions. Starting on that.



## July 31:


C++ exception handling is quite limited if you use exceptiosn by & reference. I wonder if it's better if you use new Exception?? Must test. Answer: yes, works!!! NOTE: always use "new" with exceptions!

also: no finally block (¬_¬) ...


Functions work !!!!!! but not recursively. need to separate m_pointer from instruction sequence in InstructionRunner -- should be easy: just copy the IR each time.


## August 1:

Started playing around with lex and yacc (flex and bison). Been a while, been a while.


## August 3: 

OMG I have basic compiler functionality working. It's mem-leaky as shit and crude, but i've basically got a working script language woooooooo!!!!


AAAAND -- my first bit of bad design coming back to bite me in the butt. COMPARE might be the wrong thing to have done. must think on this more. I suspect I need to move it to TESTXXX instructions that all evalutate to a bool true/false flag, put those in TMPs/register, and then go from there. So, more complicated COMPARE/TESTs, but fewer JUMP instructions then.  D'oh!


## August 7:

Great, my compiler is going a lot better now and I can generate a lot of code, but -- it's wrong. I need to move to an intermediate representation from which I will generate code.  This is basically gonna be an overly simplistic AST. That will let me do further analyses, generate better code, and start even thinkign about optimisations.

\#winning.


## August 11:

I'm basically reworking how the compiler is set up, and generating an AST now. Have most basic nodes working; need to add some more complicated stuff like loops and functions.


## August 20:

Looping now supported in the AST.

## August 21:

Add more functionality, function calls, function definitions, and flow-of-control statements.

Next big project: code generation from this AST. Can't wait!



