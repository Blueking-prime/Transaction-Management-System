# Transaction-Management-System

This Transaction Management system built in C uses the SQLite and cJSON libraries.

It allows you to perform multiple operations including:

- Create a new account:

        new <first name> <last name> <bvn> <pin>

- Deposit money into an account:

        deposit <account number> <amount>

- Withdraw money from an account:

        withdraw <account number> <pin> <amount>

- Check account balance:

        check <account number> <pin>
- Close an account:

        liquidate <account number> <pin> <bvn>

## Requirements (Install from your package manager)

- SQLite3

## Installation instructions

- Git clone the repo

        git clone https://github.com/Blueking-prime/Transaction-Management-System

- Navigate into the folder and compile, making sure to link the required libraries

        gcc *.c -l sqlite3 -o TMS

- Run the output file

        ./TMS

- Start operating in the interactive shell

You can type ```help``` in the interactive shell to view the available commands.
