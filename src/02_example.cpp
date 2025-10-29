//This files uses liquibook 
#include <book/order_book.h>
#include <cstdint>
#include <iostream>
#include <string>
/*
BUSINESS TERMS 
- Order Book: the live queue of buy (bids) and sell (asks) orders waiting to match.
- Limit Order: buy/sell at a specified price or better. May wait in the book.
- Market Order: execute immediately at best available prices (no price limit).
- Stop Price: trigger level that activates a stop/stop-limit order.
- Stop-Loss (Stop or Stop-Limit): becomes active when price hits stop; often used to cap losses.
- Quantity (order_qty): number of shares/contracts in the order.
- Price (ticks/cents): integer price (e.g., 5000 == $50.00) to avoid float errors.
- AON (All-Or-None): fill the whole order or don’t fill any part.
- IOC (Immediate-Or-Cancel): fill what you can right now; cancel the rest.
- FOK (Fill-Or-Kill): AON + IOC → fill entire order immediately or cancel entirely.
*/
class SimpleOrder {
public:
  /**
   * @param is_buy  true = buy (bid), false = sell (ask)
   * @param qty     total quantity (shares/contracts)
   * @param price   limit price in ticks (0 => market order)
   * @param id      client/order identifier
   * @param stop_price  (>0) activates stop behavior; 0 = none
   * @param all_or_none  require full fill or cancel
   * @param immediate_or_cancel execute immediately; cancel unfilled
   */
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
  /// @return true if buy, false if sell
  // Required interface - now returns actual values
  bool is_buy() const { return is_buy_; }
  /// @return total order quantity
  uint32_t order_qty() const { return quantity_; }
  /// @return price in ticks (0 => market)
  int32_t price() const { return price_; }
  /// @return instrument symbol
  const std::string &symbol() const { return symbol_; }
  /// @return stop trigger price (0 if disabled)
  int32_t stop_price() const { return stop_price_; } 
  /// @return AON flag
  bool all_or_none() const { return all_or_none_; }
  /// @return IOC flag
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
  int32_t stop_price_;       
  bool all_or_none_;         
  bool immediate_or_cancel_; 
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
