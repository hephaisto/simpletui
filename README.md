simpletui
=========

a simple text user interface (wrapper for CDK)

This library is intended to be used for small configuration tools, that can show the user various choices instead of requiring editing of config files or specifying the correct command-line parameters.
It is not intendet to create complex user-interfaces. Instead, it is more like an ugly library-version of the dialog utility for shell scripts.

dependencies
------------

this library depends on CDK (curses development kit)

features
--------

* completely synchronous design
* exceptions for user aborts
* using STL datatypes as parameters and return types
* multiple types message boxes
	* msgbox with ok button
	* string input box
	* password input box
	* yesno box with boolean result
	* msgbox with multiple buttons (`choiceFew`)
	* msgbox with multiple vertically aligned buttons and scrollbar

installing
----------

	make
	sudo make install

using
-----

	// include header file
	#include <simpletui.hpp>

	// initialise and use
	Simpletui tui;
	if(tui.yesno("continue?"))
		tui.msg("hello world");


see `simpletui_test.cpp` for more examples.

notes
-----

enabling C++0x-features makes using this library a lot easier, because you can use initialiser lists.
