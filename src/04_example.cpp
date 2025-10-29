/**
 * ============================================================================
 * LIQUIBOOK ORDER MATCHING ENGINE - EXAMPLE 4
 * Event Listeners and Callbacks
 * ============================================================================
 *
 * This example demonstrates how to use listeners to get real-time notifications
 * about order events (accepts, fills, cancels, etc.) in the Liquibook matching
 * engine.
 *
 * BUSINESS TERMS GLOSSARY:
 * ============================================================================
 *
 * ORDER BOOK:
 *   A list of buy and sell orders for a specific asset (like a stock). The
 *   order book matches buyers with sellers automatically.
 *
 * BID:
 *   A buy order. "I want to BUY 100 shares at $50 or less."
 *
 * ASK (or OFFER):
 *   A sell order. "I want to SELL 100 shares at $50 or more."
 *
 * LIMIT ORDER:
 *   An order with a specific price. Won't execute unless the price matches.
 *   Example: "Buy 100 shares at $50 maximum."
 *
 * MARKET ORDER:
 *   An order without a price limit. Executes immediately at best available
 * price. Example: "Buy 100 shares at whatever price is available."
 *
 * MATCHING:
 *   When a buy order and sell order have compatible prices, the engine
 *   "matches" them and creates a trade.
 *
 * FILL:
 *   When an order is executed (matched with another order). An order can be:
 *   - Fully filled: All quantity traded
 *   - Partially filled: Some quantity traded, rest remains in book
 *
 * SPREAD:
 *   The difference between the best bid price and best ask price.
 *   Example: Best bid is $49, best ask is $51 → Spread is $2
 *
 * ACCEPT:
 *   Order is valid and added to the order book (but not yet filled).
 *
 * REJECT:
 *   Order is invalid and not added to the book (e.g., invalid price).
 *
 * CANCEL:
 *   Remove an order from the book before it's filled.
 *
 * REPLACE:
 *   Modify an existing order's price or quantity.
 *
 * LIQUIDITY:
 *   The number of orders available in the book. More orders = more liquidity.
 *
 * ============================================================================
 */

#include <SimpleOrder.h>
#include <book/order_book.h>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

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

/**
 * ============================================================================
 * MAIN FUNCTION - Comprehensive Trading Simulation
 * ============================================================================
 *
 * This simulates a day of trading with various scenarios:
 * 1. Opening trades (immediate matches)
 * 2. Orders that sit in the book (no immediate match)
 * 3. Partial fills
 * 4. Order cancellations
 * 5. Market orders
 */
int main() {
  // Create the order book - this is the "matching engine"
  liquibook::book::OrderBook<SimpleOrder *> order_book;

  // Create and attach our listener to receive notifications
  MyOrderListener listener;
  order_book.set_order_listener(&listener);

  std::cout << "     LIQUIBOOK TRADING SIMULATION - EXAMPLE 4              "
            << std::endl;
  std::cout << "     Testing Event Listeners & Callbacks                   "
            << std::endl;

  // ========================================================================
  // SCENARIO 1: Perfect Match (Immediate Trade)
  // ========================================================================
  std::cout << "\n--- SCENARIO 1: Perfect Match ---" << std::endl;
  std::cout << "Alice wants to SELL 100 shares at $50" << std::endl;
  std::cout << "Bob wants to BUY 100 shares at $50" << std::endl;
  std::cout << "Expected: Immediate trade at $50\n" << std::endl;

  SimpleOrder *sell1 = new SimpleOrder(false, 100, 5000, "SELL_001");
  order_book.add(sell1);
  order_book.perform_callbacks();

  SimpleOrder *buy1 = new SimpleOrder(true, 100, 5000, "BUY_001");
  order_book.add(buy1);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 2: Partial Fill
  // ========================================================================
  std::cout << "\n--- SCENARIO 2: Partial Fill ---" << std::endl;
  std::cout << "Charlie wants to SELL 200 shares at $51" << std::endl;
  std::cout << "Diana wants to BUY only 75 shares at $51" << std::endl;
  std::cout << "Expected: Diana gets all 75, Charlie has 125 left\n"
            << std::endl;

  SimpleOrder *sell2 = new SimpleOrder(false, 200, 5100, "SELL_002");
  order_book.add(sell2);
  order_book.perform_callbacks();

  SimpleOrder *buy2 = new SimpleOrder(true, 75, 5100, "BUY_002");
  order_book.add(buy2);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 3: No Match (Orders Sit in Book)
  // ========================================================================
  std::cout << "\n--- SCENARIO 3: No Match (Spread) ---" << std::endl;
  std::cout << "Eve wants to BUY at $48 (too low)" << std::endl;
  std::cout << "Frank wants to SELL at $53 (too high)" << std::endl;
  std::cout << "Expected: Both orders accepted but no trade\n" << std::endl;

  SimpleOrder *buy3 = new SimpleOrder(true, 100, 4800, "BUY_003");
  order_book.add(buy3);
  order_book.perform_callbacks();

  SimpleOrder *sell3 = new SimpleOrder(false, 100, 5300, "SELL_003");
  order_book.add(sell3);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 4: Market Order (Takes Best Price)
  // ========================================================================
  std::cout << "\n--- SCENARIO 4: Market Order ---" << std::endl;
  std::cout << "Grace places MARKET order to BUY 125 shares" << std::endl;
  std::cout << "Expected: Matches with Charlie's remaining 125 @ $51\n"
            << std::endl;

  SimpleOrder *market_buy = new SimpleOrder(true, 125, 0, "MARKET_001");
  order_book.add(market_buy);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 5: Order Cancellation
  // ========================================================================
  std::cout << "\n--- SCENARIO 5: Order Cancellation ---" << std::endl;
  std::cout << "Eve cancels her BUY order at $48" << std::endl;
  std::cout << "Expected: Successful cancellation\n" << std::endl;

  order_book.cancel(buy3);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 6: Multiple Partial Fills
  // ========================================================================
  std::cout << "\n--- SCENARIO 6: Multiple Partial Fills ---" << std::endl;
  std::cout << "Henry places large BUY order for 300 shares at $53"
            << std::endl;
  std::cout << "Expected: Matches Frank's 100 first, waits for more\n"
            << std::endl;

  SimpleOrder *buy4 = new SimpleOrder(true, 300, 5300, "BUY_004");
  order_book.add(buy4);
  order_book.perform_callbacks();

  std::cout << "\nIvy adds SELL order for 150 shares at $53" << std::endl;
  std::cout << "Expected: Matches with Henry's remaining order\n" << std::endl;

  SimpleOrder *sell4 = new SimpleOrder(false, 150, 5300, "SELL_004");
  order_book.add(sell4);
  order_book.perform_callbacks();

  // ========================================================================
  // SCENARIO 7: Aggressive Market Taker
  // ========================================================================
  std::cout << "\n--- SCENARIO 7: Aggressive Buyer ---" << std::endl;
  std::cout << "Jack places high BID at $55 to attract sellers" << std::endl;
  std::cout << "Kate places SELL at $52" << std::endl;
  std::cout << "Expected: Trade at $52 (seller's price - price improvement!)\n"
            << std::endl;

  SimpleOrder *buy5 = new SimpleOrder(true, 100, 5500, "BUY_005");
  order_book.add(buy5);
  order_book.perform_callbacks();

  SimpleOrder *sell5 = new SimpleOrder(false, 100, 5200, "SELL_005");
  order_book.add(sell5);
  order_book.perform_callbacks();

  // ========================================================================
  // Final Summary
  // ========================================================================
  std::cout << "                    SIMULATION COMPLETE                     "
            << std::endl;

  std::cout << "\n Key Learnings:" << std::endl;
  std::cout << "   ✓ Orders are accepted before being filled" << std::endl;
  std::cout << "   ✓ Trades happen when buy and sell prices match" << std::endl;
  std::cout << "   ✓ Partial fills occur when quantities don't match"
            << std::endl;
  std::cout << "   ✓ Market orders execute at best available price"
            << std::endl;
  std::cout << "   ✓ Unfilled orders sit in book waiting for matches"
            << std::endl;
  std::cout << "   ✓ Price improvement benefits the taker" << std::endl;

  // Clean up all orders
  delete sell1;
  delete buy1;
  delete sell2;
  delete buy2;
  delete buy3;
  delete sell3;
  delete market_buy;
  delete buy4;
  delete sell4;
  delete buy5;
  delete sell5;

  return 0;
}
