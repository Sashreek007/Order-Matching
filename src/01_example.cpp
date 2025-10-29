#include <cstdint>
#include <iostream>
#include <string>

class SimpleOrder {
public:
  // Constructor (equal to __init__ in python)
  SimpleOrder(bool is_buy, uint32_t qty, int32_t price, std::string id)
      : is_buy_(is_buy), quantity_(qty), price_(price), order_id_(id) {}

  // These fields are required by Liquibook
  // Getters
  bool is_buy() const { return is_buy_; }

  uint32_t order_qty() const { return quantity_; }

  int32_t price() const { return price_; }

  const std::string &symbol() const { return symbol_; }

  // Custom fields (optional)
  std::string order_id_;
  std::string symbol_ = "AAPL"; // Default symbol

private:
  bool is_buy_;
  uint32_t quantity_;
  int32_t price_;
};

int main() {
  SimpleOrder buy_order(true, 100,
                        4000, // $40
                        "001");

  std::cout << "Order Created!" << std::endl;
  std::cout << "Type: " << (buy_order.is_buy() ? "BUY" : "SELL") << std::endl;
  std::cout << "Quantity: " << buy_order.order_qty() << std::endl;
  std::cout << "Price: $" << (buy_order.price() / 100.0) << std::endl;
  std::cout << "Symbol: " << buy_order.symbol() << std::endl;
  std::cout << "Order ID: " << buy_order.order_id_ << std::endl;

  return 0;
}
