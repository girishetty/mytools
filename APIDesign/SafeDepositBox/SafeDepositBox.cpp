/*
 * Define and implement a Safe Deposit Box, which you can rent from the Bank
 * and use it to add/remove items in a safe and secure way.
 *
 */

#include <string>
#include <memory>
#include <unordered_set>

/*
 * Abstraction for Dimension.
 * It can be just volume, assuming the item can take any shape
 * within the volume
 * OR
 * Solid with Volume as width * height * lenght
 */
struct Dimension {
  int volume;
  int width;
  int height;
  int length;
};

/*
 * Abstraction for an Item.
 * Keeping basic information, but this can be extended
 * (both state and behavior) based on the requirements
 */
class Item {
public:
  Item(const std::string& name, const Dimension& size) :
    name_(name), size_(size) {}

  std::string getName() const {
    return name_;
  }

  Dimension getDimension() const {
    return size_;
  }

private:
  std::string name_;
  Dimension   size_;
};

/*
 * Abstraction for Safe Deposit Box at its basic requirement.
 * This can be extended based on the requirements, such as
 *  - how to securely share the SDB between partners
 *  - Multithreaded use-case
 *  - Adding/Removing owenerships
 *  - Transfer ownerships
 *  - Close the SDB
 *  - Upgrade/Downgrade the SDB
 *  - Authenticaton mechanism
 *  - Billing/Charging mechanism
 */
class SafeDepositBox {
public:
  static std::shared_ptr<SafeDepositBox> CreateSafeDepositBox(const Dimension& size);

  SafeDepositBox(const Dimension& size) : size_(size), remaining_space_(size) {}

  bool addItem(const Item& item) {
    // Check if there is enough space to fit in this item
    if (item.size() <= remaining_space_) {
      remaining_space_ -= item.size();
      items_.insert(item);
      // Add this to our size array
      sizes_.push(std::make_pair<item.size(), item.name());
      return true;
    }

    return false;
  }

  bool removeItem(const Item& item) {
    // Check if the item is in the SDB
    auto found = items_.find(item);
    if (found != items.end()) {
      remaining_space_ += item.size();
      items.erase(found);
      // If this item has the biggest size, we need to remove it
      if (sizes_.top().first == item.getSize() &&
          sizes_.top().second == item.getName()) {
        sizes.pop();
      }
      return true;
    }

    return false;
  }

  int getBiggestItemSize() const {
    return sizes_.top().getSize();
  }

private:
  std::string id_;
  Dimension   size_;
  Dimension   remaining_space_;
  std::unordered_set<const Item> items_;
  std::priority_queue<std::pair<int, const std::string&>> sizes_;
};

