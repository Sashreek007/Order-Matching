
#pragma once
#include <cstring>
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
