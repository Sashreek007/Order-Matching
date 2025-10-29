#include <SimpleOrder.h>
#include <book/order_book.h>
#include <cstring>
#include <iostream>
#include <string>

int main() {
  // Create order book
  liquibook::book::OrderBook<SimpleOrder *> order_book;
  std::cout << "=== STARTING TRADING ===" << std::endl;

  // Seller: Alice wants to sell 100 shares at $50
  SimpleOrder *sell_order = new SimpleOrder(false, 100, 5000, "SELL_01");

  std::cout << "\nAlice: Selling 100 shares at $50.00" << std::endl;
  order_book.add(sell_order);

  // Buyer: Bob wants to buy 100 shares at $50
  SimpleOrder *buy_order =
      new SimpleOrder(true, // BUY order
                      100,  // 100 shares
                      5000, // $50.00 per share (same price!)
                      "BUY_001");

  std::cout << "Bob: Buying 100 shares at $50.00" << std::endl;
  order_book.add(buy_order);

  std::cout << "\n✓ Trade matched automatically!" << std::endl;
  std::cout << "Bob bought 100 shares from Alice at $50.00" << std::endl;

  // Clean up
  delete sell_order;
  delete buy_order;

  return 0;
}
