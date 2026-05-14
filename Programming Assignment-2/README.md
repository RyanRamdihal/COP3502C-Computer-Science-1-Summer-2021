# COP 3502C Assignment 2 — Super Slow Supermarket

**Author:** Ryan Ramdihal
**Course:** COP 3502C — Computer Science I
**Topic:** Linked Lists, Queues, and Dynamic Memory Allocation

## Overview

This program simulates a "Super Slow Supermarket" that has only one clerk but multiple customer lines. Customers queue up in lines numbered 1 through 12, and the single clerk surveys all of the lines after finishing with each customer. The clerk always picks the next customer from the front of whichever line currently has a customer with the **fewest items**. Ties are broken by choosing the customer in the **smaller-numbered line**.

The program reads customer information from an input file, simulates the checkout process, and prints the time at which each customer finishes checking out.

## Problem Details

- Each customer has a name, an arrival time, a line number, and an item count.
- Checkout time per customer = `30 + 5 × (number of items)` seconds.
- When selecting the next customer, the clerk ignores customers who have not arrived yet (arrival time greater than current simulation time).
- If every non-empty line's front customer arrives after the current time, the clerk fast-forwards the clock to the earliest arrival and picks that customer.
- All arrival times are unique, and all customer names are unique.

## Files

- `assignment2.c` — C source file containing the full implementation.
- `assignment2input.txt` — Default input file the program reads when no command-line argument is provided.

## Input Format

The first line contains a positive integer `c` (`c ≤ 25`), the number of test cases.

For each test case:
- The first line contains a positive integer `n` (`n ≤ 500,000`), the number of customers.
- The next `n` lines each contain: `time line_number name number_of_items`
  - `time` — arrival time in seconds (`t ≤ 10^9`)
  - `line_number` — line the customer joins (`1 ≤ m ≤ 12`)
  - `name` — uppercase string of 1–9 letters
  - `number_of_items` — items in cart (`x ≤ 100`)

### Sample Input

```
2
5
10 1 STEVEN 12
12 6 SAHAR 8
13 1 JENNY 40
22 6 JERMAINE 39
100000 12 AISA 53
6
100 1 A 100
200 2 B 99
300 3 C 98
400 4 D 97
500 5 E 96
600 6 F 95
```

## Output Format

For each customer, in the order they finish checking out, the program prints:

```
CUSTOMER from line X checks out at time T.
```

### Sample Output

```
STEVEN from line 1 checks out at time 100.
SAHAR from line 6 checks out at time 170.
JERMAINE from line 6 checks out at time 395.
JENNY from line 1 checks out at time 625.
AISA from line 12 checks out at time 100295.
A from line 1 checks out at time 630.
F from line 6 checks out at time 1135.
E from line 5 checks out at time 1645.
D from line 4 checks out at time 2160.
C from line 3 checks out at time 2680.
B from line 2 checks out at time 3205.
```

## Compilation and Usage

Compile with `gcc`:

```
gcc assignment2-1.c -o assignment2
```

Run using the default input file `assignment2input.txt` in the current directory:

```
./assignment2
```

Or pass a custom input file as a command-line argument:

```
./assignment2 my_input.txt
```

The grader runs the program on the Eustis server, so the code is written to compile and run there.

## Implementation Notes

The program follows the assignment's required design:

- A `customer` struct stores the name, item count, line number, and arrival time.
- A `node` struct holds a pointer to a `customer` and a pointer to the next node, forming a singly linked list.
- A `queue` struct keeps `front`, `back`, and `size`, implemented as a linked list of nodes.
- The 12 checkout lines are stored as a fixed-size array `queue queue_list[12]`.
- The queue supports `enqueue`, `dequeue`, `front` (peek), and `empty`.
- Memory is dynamically allocated for both customers and nodes. Nodes are freed on dequeue; customers are freed after the checkout time is calculated and printed.
- Because there is only one clerk, all customers can be enqueued up front before any checkout begins.

## Selection Logic

After each checkout, the program scans the front of every non-empty line and:

1. Skips lines whose front customer has not arrived yet.
2. Among the remaining candidates, picks the one with the fewest items (ties broken by smaller line number, which is naturally handled by iterating lines 1 through 12 in order).
3. If no line has an arrived customer, the simulation clock jumps to the earliest pending arrival and repeats the scan.

## Rubric Highlights

- Must compile on Eustis.
- Must use dynamic memory allocation (otherwise: 0).
- Must free all allocated memory.
- Hardcoding the output results in a 0.
- Poorly indented or unreadable code receives a 10% penalty.
