# juicy_exchang
This repo is inspired by UVa1598, a simple exchange system.

## Main Features
 - [] Providing an interface to exchange products
 - [] Providing a BigNum and Priority Queue Utility
 
 ##Interface
The program is written in C++ for it's easy to port to other languages. The `juicyExchange.h` contains the API functions definition.

To start with the exchange, create an Exchange Object.
Then, use `quote()` to get quote, use `order(Order)` to push an order, use `Cancel(Order)` to cancel an existing order. 
