#include <book/order_book.h>
#include <cstdint>
#include <iostream>
#include <string>

class SimpleOrder {
public:
  // Constructor with ALL order properties
  SimpleOrder(bool is_buy, uint32_t qty, int32_t price, std::string id,
              int32_t stop_price = 0, // Optional parameters
              bool all_or_none = false, bool immediate_or_cancel = false)
      : is_buy_(is_buy), quantity_(qty), price_(price), order_id_(id),
        stop_price_(stop_price) // Store it!
        ,
        all_or_none_(all_or_none), immediate_or_cancel_(immediate_or_cancel) {
    std::cout << "Created" << getOrderType() << "order:" << order_id_
              << std::endl;
  }

  // Required interface - now returns actual values
  bool is_buy() const { return is_buy_; }
  uint32_t order_qty() const { return quantity_; }
  int32_t price() const { return price_; }
  const std::string &symbol() const { return symbol_; }
  int32_t stop_price() const { return stop_price_; } // Returns stored value
  bool all_or_none() const { return all_or_none_; }
  bool immediate_or_cancel() const { return immediate_or_cancel_; }
  std::string getOrderType() const {
    std::string type = "";

    if (price_ == 0) {
      type = "MARKET";
    } else {
      type = "LIMIT";
    }

    if (stop_price_ > 0) {
      type = "STOP-LOSS " + type;
    }

    if (all_or_none_ && immediate_or_cancel_) {
      type += " (FILL-OR-KILL)";
    } else if (all_or_none_) {
      type += " (ALL-OR-NONE)";
    } else if (immediate_or_cancel_) {
      type += " (IMMEDIATE-OR-CANCEL)";
    }

    return type;
  }
  std::string order_id_;
  std::string symbol_ = "AAPL";

private:
  bool is_buy_;
  uint32_t quantity_;
  int32_t price_;
  int32_t stop_price_;       // NEW: Store stop price
  bool all_or_none_;         // NEW: Store AON flag
  bool immediate_or_cancel_; // NEW: Store IOC flag};
};
int main() {

  liquibook::book::OrderBook<SimpleOrder *> order_book;

  std::cout << "=== Creating Different Order Types ===" << std::endl;

  // Example 1: Simple limit order (default parameters)
  SimpleOrder *limit_order = new SimpleOrder(true, 100, 5000, "LIMIT_001");

  // Example 2: Market order (price = 0)
  SimpleOrder *market_order = new SimpleOrder(true, 100, 0, "MARKET_001");

  // Example 3: Stop-loss order
  SimpleOrder *stop_order =
      new SimpleOrder(false, // Sell
                      100,   // Quantity
                      4500,  // Limit price: $45
                      "STOP_001",
                      5000 // Stop price: trigger when price hits $50
      );

  // Example 4: All-or-none order
  SimpleOrder *aon_order = new SimpleOrder(true, // Buy
                                           100,  // Quantity
                                           5000, // Price
                                           "AON_001",
                                           0,   // No stop price
                                           true // All-or-none = true
  );

  // Example 5: Immediate-or-cancel order
  SimpleOrder *ioc_order = new SimpleOrder(true, // Buy
                                           100,  // Quantity
                                           5000, // Price
                                           "IOC_001",
                                           0,     // No stop price
                                           false, // Not all-or-none
                                           true   // Immediate-or-cancel = true
  );

  // Example 6: Fill-or-kill order (AON + IOC)
  SimpleOrder *fok_order = new SimpleOrder(true, // Buy
                                           100,  // Quantity
                                           5000, // Price
                                           "FOK_001",
                                           0,    // No stop price
                                           true, // All-or-none = true
                                           true  // Immediate-or-cancel = true
  );

  std::cout << "\n=== Adding Orders to Book ===" << std::endl;

  order_book.add(limit_order);
  order_book.add(market_order);
  order_book.add(stop_order);
  order_book.add(aon_order);
  order_book.add(ioc_order);
  order_book.add(fok_order);

  std::cout << "\n=== Order Book Ready ===" << std::endl;

  // Clean up
  delete limit_order;
  delete market_order;
  delete stop_order;
  delete aon_order;
  delete ioc_order;
  delete fok_order;

  return 0;
}
