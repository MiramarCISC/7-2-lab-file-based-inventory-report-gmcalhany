#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    return quantity >= 0;
}

bool isValidPrice(double price) {
    return price >= 0;
}

double calculateItemValue(const InventoryItem& item) {
    if (!isValidQuantity(item.quantity) || !isValidPrice(item.price)) {
        return 0.0;
    }

    return item.quantity * item.price;
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    if (items == nullptr || maxItems <= 0) {
        return 0;
    }

    ifstream input(filename);

    if (!input.is_open()) {
        return 0;
    }

    int count = 0;

    while (count < maxItems &&
           input >> items[count].sku
                 >> items[count].name
                 >> items[count].quantity
                 >> items[count].price) {

        if (isValidQuantity(items[count].quantity) &&
            isValidPrice(items[count].price)) {

            count++;
        }
    }

    input.close();

    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    if (items == nullptr || count < 0) {
        return false;
    }

    ofstream output(filename);

    if (!output.is_open()) {
        return false;
    }

    double total = 0.0;

    for (int i = 0; i < count; i++) {
        double value = calculateItemValue(items[i]);

        output << items[i].sku << " "
               << items[i].name << " "
               << fixed << setprecision(2)
               << value << endl;

        total += value;
    }

    output << "Total inventory value: "
           << fixed << setprecision(2)
           << total << endl;

    output.close();

    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return 0.0;
    }

    double total = 0.0;

    for (int i = 0; i < count; i++) {
        total += calculateItemValue(items[i]);
    }

    return total;
}

int findItemBySku(const InventoryItem items[], int count, string sku) {
    if (items == nullptr || count <= 0) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }

    return -1;
}

int findHighestValueItemIndex(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return -1;
    }

    int highestIndex = 0;
    double highestValue = calculateItemValue(items[0]);

    for (int i = 1; i < count; i++) {
        double value = calculateItemValue(items[i]);

        if (value > highestValue) {
            highestValue = value;
            highestIndex = i;
        }
    }

    return highestIndex;
}
