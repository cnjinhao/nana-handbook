# Nana Handbook
2017 Nana C++ Library Developer Community

This documentation introduces some basic knowledges about rapid GUI programming with Nana C++ Library.

Now let's get started!

## Table of Contents

INTRODUCTION

  * [Why is Modern C++?](#why-is-modern-c)
  * [Hello World!](#hello-world)

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

It sends the text "Hello World!" and a newline/line flush(endl) to the console for output and display.

Now, let's start 3 HelloWorld examples with Nana. Each example displays the text "Hello World!" with different functions.

### EXAMPLE 1

Display "Hello World!" by popping up a message box.

```cpp
//It instructs the compiler to include the core part of Nana,
//this header file contains some class and function definitions.
#include <nana/gui.hpp>

int main()
{
	//A message box object is created, titiled "Example".
	nana::msgbox msg{"Example"};

	//Sends the text "Hello World!" to the message box
	//output buffer.
	msg << "Hello World!";

	//Pops up a messagebox. The `show()` blocks the execution
	//until the message box is closed.
	msg.show();
}
```
> Q: Why does Nana provide a class for the message box rather than a function like Win32 MessageBox?

Because the class msgbox is designed to be a function object. Following example illustrates the msgbox to be used as a handler that pops up a message box when form is clicked.

```cpp
#include <nana/gui.hpp>

int main()
{
	nana::form fm;
	fm.events().click(nana::msgbox{fm, "Example"}<<"The form is clicked");
	fm.show();
	nana::exec();
}
```

### EXAMPLE 2

Create a form and a label widget, the label displays the text "Hello World!". A form is a popup window.

```cpp
#include <nana/gui.hpp>

//Includes the label header
#include <nana/gui/widgets/label.hpp>

int main()
{
	//Brings all nana names into main scope.
	using namespace nana;

	//Create a form.
	form fm;

	//Create a label on the form, at position (1, 20) with size 100x30
	label lb{fm, rectangle{10, 20, 100, 30}};

	//Show the text "Hello World!"
	lb.caption("Hello World!");

	//Show the form
	fm.show();

	//where main() passes the control to Nana, and exec() will return
	//if the form is closed.
	exec();
}
```

This example also shows a basic structure of Nana's application. Prepare at least one form object, then call `exec()`. The `exec()` listens for a user input and dispatches the input to correspending modules for processing.