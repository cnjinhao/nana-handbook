# Nana Handbook
2017 Nana C++ Library Developer Community

This documentation introduces some basic knowledges about rapid GUI programming with Nana C++ Library.

Now let's get started!

## Table of Contents

INTRODUCTION

  * Why is Modern C++?
  * Hello, world!

# INTRODUCTION
Nana is a cross-platform library written in modern C++, which is designed to make things simple and intuitive.

## WHY IS MODERN C++?
By "Modern C++" we mean C++11/14/17, the philosophy of modern C++ is to write simple and intuitive code using modern C++ features:

  * RAII
  * Standard library
  * Templates and metaprogramming
  * Exceptions

Resource acquisition is initialization(RAII) is an important technique of C++, it reduces a huge complexity for the application of Nana. For a developer, he would not take care about how a Nana class acquires/releases resource and also don't warry about the resource leak when an exception is thrown.

The standard library is used not only the internal implementation of Nana but also in the design of programming interfaces. Smart pointers and containers used for the return value type and parameter types greatly reduce the complexity of memory management. For example, passing multi values with an intializer-list to a function that parameter is a container.

Templates and metaprogramming have nature ability for library design. By using these techniques, it is possible to make more general and intuitive interfaces/functions/class. For example, Nana's event handler is designed to have a parameter to receive the event argument, but developers can pass a function without the parameter to the event register when they want to ignore the event argument.

Using exceptions for error handling makes library simpler, cleaner and less likely to miss errors. It is safe because of RAII.


## HELLO WORLD!

A traditional HelloWorld program in C++ book is looked like this

```cpp
#include <iostream>

int main()
{
	std::cout << "Hello World!" << std::endl;
}
```

It send the text "Hello World!" and a newline/line flush(endl) to the console for output and disply.

Let's start a HelloWorld program with Nana.

```cpp
1	#include <nana/gui.hpp>
2
3	int main()
4	{
5		nana::msgbox msg{"Example"};
6		msg << "Hello World!";
7		msg.show();
8	}
```

It pops up a message box and displays text "Hello World!". Then click the OK button, the program exits.

**Line 1** It instructs the compiler to include the core part of Nana, it contains some class and function definitions.