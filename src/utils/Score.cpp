#include "Score.hpp"
#include "Log.hpp"

#include <vector>
#include <fstream>
#include <memory>

const char* scoreFile = "Score.bin";

int32_t bytes2Int(const std::vector<uint8_t>& src, size_t& offset) {
	int32_t value = (src[offset] << 24) | (src[offset + 1] << 16) | (src[offset + 2] << 8) | (src[offset + 3]);
	offset += sizeof(value);
	return value;
}

void int2Bytes(int32_t value, std::vector<uint8_t>& dest, size_t& offset) {
	dest[offset] = (value >> 24) & 0xFF;
	dest[offset + 1] = (value >> 16) & 0xFF;
	dest[offset + 2] = (value >> 8) & 0xFF;
	dest[offset + 3] = (value) & 0xFF;
	offset += sizeof(value);
}

bool write_binary_file(const std::string& filename, std::vector<uint8_t>& data, size_t size) {
	std::ofstream output(filename, std::ios::out | std::ios::binary);
	if (!output.is_open())
		return false;
	output.write(reinterpret_cast<const char*>(data.data()), size);
	output.close();
	return true;
}

std::unique_ptr<std::vector<uint8_t>> read_binary_file(const std::string& filename) {
	std::ifstream input(filename, std::ios::binary);
	if (!input.is_open())
		return nullptr;
	input.seekg(0, std::ios_base::end);
	size_t length = input.tellg();
	input.seekg(0, std::ios_base::beg);

	std::unique_ptr<std::vector<uint8_t>> data = std::make_unique<std::vector<uint8_t>>(length);
	input.read(reinterpret_cast<char*>(data->data()), length);
	input.close();
	return data;
}

Score readScore() {
	const auto& data = read_binary_file(scoreFile);
	Score score;
	if (data == nullptr) {
		return score;
	}
	size_t offset = 0;
	score.score = bytes2Int(*data, offset);
	score.hightScore = bytes2Int(*data, offset);

	return score;
}

void writeScore(int32_t hightScore) {
	std::vector<uint8_t> dst(2 * sizeof(int32_t));
	size_t offset = 0;
	Score score = readScore();
	int2Bytes(hightScore, dst, offset);
	int2Bytes((hightScore > score.hightScore ? hightScore : score.hightScore), dst, offset);
	write_binary_file(scoreFile, dst, dst.size());
}