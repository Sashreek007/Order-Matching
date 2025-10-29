
#pragma once
/**
 * ============================================================================
 * CLASS: MyOrderListener
 * ============================================================================
 * Receives notifications about order events from Liquibook.
 *
 * This is the "callback" mechanism - Liquibook calls these methods to tell you
 * what's happening with your orders in real-time.
 *
 * Think of it like a news reporter announcing trades on a stock exchange!
 */
class MyOrderListener : public liquibook::book::OrderListener<SimpleOrder *> {
public:
  /**
   * Called when an order is ACCEPTED into the order book
   *
   * This means:
   * - The order is valid
   * - It's been added to the book
   * - It's waiting to be matched
   *
   * @param order  The order that was accepted
   */
  void on_accept(SimpleOrder *const &order) override {
    std::cout << "✓ Order ACCEPTED: " << order->order_id_ << " ("
              << (order->is_buy() ? "BUY" : "SELL") << " " << order->order_qty()
              << " @ $" << std::fixed << std::setprecision(2)
              << (order->price() / 100.0) << ")" << std::endl;
  }

  /**
   * Called when an order is FILLED (a trade happens!)
   *
   * This is the most important event - it means a buyer and seller
   * have been matched and a trade has executed.
   *
   * @param order   Your order that got filled
   * @param matched_order The counterparty order that matched
   * @param fill_qty The quantity filled
   * @param fill_price The price of the fill
   *
   * NOTE: An order can have multiple fills if it's matched with multiple orders
   */
  void on_fill(SimpleOrder *const &order, SimpleOrder *const &matched_order,
               liquibook::book::Quantity fill_qty,
               liquibook::book::Price fill_price) override {
    double price_per_unit = fill_price / 100.0;
    double total_value = (fill_qty * fill_price) / 100.0;

    std::cout << "\n TRADE EXECUTED!" << std::endl;
    std::cout << "   Order ID:      " << order->order_id_ << std::endl;
    std::cout << "   Matched with:  " << matched_order->order_id_ << std::endl;
    std::cout << "   Side:          " << (order->is_buy() ? "BUY" : "SELL")
              << std::endl;
    std::cout << "   Quantity:      " << fill_qty << " shares" << std::endl;
    std::cout << "   Price:         $" << std::fixed << std::setprecision(2)
              << price_per_unit << " per share" << std::endl;
    std::cout << "   Total Value:   $" << total_value << std::endl;
    std::cout << std::endl;
  }

  /**
   * Called when an order is REJECTED
   *
   * This means the order was invalid and NOT added to the book.
   * Reasons might include:
   * - Invalid price (negative, or zero for limit order)
   * - Invalid quantity (zero or negative)
   *
   * @param order   The order that was rejected
   * @param reason  Why it was rejected
   */
  void on_reject(SimpleOrder *const &order, const char *reason) override {
    std::cout << "✗ Order REJECTED: " << order->order_id_
              << " - Reason: " << reason << std::endl;
  }

  /**
   * Called when an order is CANCELED
   *
   * This means the order was removed from the book before being filled.
   * You can only cancel orders that haven't been fully filled yet.
   *
   * @param order  The order that was canceled
   */
  void on_cancel(SimpleOrder *const &order) override {
    std::cout << "✗ Order CANCELED: " << order->order_id_ << std::endl;
  }

  /**
   * Called when a CANCEL request is REJECTED
   *
   * This happens when you try to cancel an order that:
   * - Doesn't exist
   * - Has already been fully filled
   * - Has already been canceled
   *
   * @param order   The order you tried to cancel
   * @param reason  Why the cancel was rejected
   */
  void on_cancel_reject(SimpleOrder *const &order,
                        const char *reason) override {
    std::cout << "✗ Cancel REJECTED: " << order->order_id_
              << " - Reason: " << reason << std::endl;
  }

  /**
   * Called when an order is REPLACED (modified)
   *
   * You can modify an existing order's price or quantity.
   * This is faster than canceling and creating a new order.
   *
   * @param order       The order that was modified
   * @param size_delta  Change in quantity (+50 = add 50, -20 = reduce 20)
   * @param new_price   The new price for the order
   */
  void on_replace(SimpleOrder *const &order, const int64_t &size_delta,
                  liquibook::book::Price new_price) override {
    std::cout << " Order REPLACED: " << order->order_id_ << std::endl;
    std::cout << "   Quantity change: " << (size_delta >= 0 ? "+" : "")
              << size_delta << std::endl;
    std::cout << "   New price: $" << std::fixed << std::setprecision(2)
              << (new_price / 100.0) << std::endl;
  }

  /**
   * Called when a REPLACE request is REJECTED
   *
   * This happens when you try to replace an order that:
   * - Doesn't exist
   * - Has already been fully filled
   * - The new price/quantity is invalid
   *
   * @param order   The order you tried to replace
   * @param reason  Why the replace was rejected
   */
  void on_replace_reject(SimpleOrder *const &order,
                         const char *reason) override {
    std::cout << "✗ Replace REJECTED: " << order->order_id_
              << " - Reason: " << reason << std::endl;
  }
};
