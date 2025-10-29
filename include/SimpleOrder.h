
#pragma once
#include <cstring>
#include <iostream>
#include <string>
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
