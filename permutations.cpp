#include <vector>
#include <iostream>

void generatePermutationsH(std::vector<int>& nums, std::vector<int>& indices) {
    if (indices.size() == nums.size()) {
        std::cout << "Printing: ";
        for (auto i : indices) {
            std::cout << nums[i];
        }
        std::cout << std::endl;
        return;
    }
    for (int i=0; i<nums.size(); i++) {
        if (std::find(indices.begin(), indices.end(), i) != indices.end()) continue;
        indices.push_back(i);
        generatePermutationsH(nums, indices);
        indices.pop_back();
    }
}

void generatePermutations(std::vector<int>& nums) {

    std::vector<int> indices;
    generatePermutationsH(nums, indices);
}

int main() {

    long num;

    std::cin >> num;

    std::string s = std::to_string(num);
    std::vector<int> nums;
    for (auto c : s) {
        nums.push_back(static_cast<int>(c - '0'));
    }

    generatePermutations(nums);

}
