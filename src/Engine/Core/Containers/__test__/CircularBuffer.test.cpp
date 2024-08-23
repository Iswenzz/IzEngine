#include "Core/Tests/Base.hpp"

#include "Core/Containers/CircularBuffer.hpp"

TEST(CircularBuffer, SizeAndOffset)
{
	CircularBuffer<int, 1000> buffer;

	for (int i = 1; i < buffer.Max(); i++)
		buffer.Add(i);

	EXPECT_EQ(buffer.Size, 999);
	EXPECT_EQ(buffer.Offset, 999);

	buffer.Add(0);

	EXPECT_EQ(buffer.Size, 1000);
	EXPECT_EQ(buffer.Offset, 0);
}

TEST(CircularBuffer, Average)
{
	CircularBuffer<int, 10> buffer;

	buffer.Add(10);
	buffer.Add(20);
	buffer.Add(30);
	buffer.Add(40);
	buffer.Add(50);

	EXPECT_EQ(buffer.Average(), 30);
	buffer.Add(60);
	EXPECT_EQ(buffer.Average(), 35);
	buffer.Add(70);
	EXPECT_EQ(buffer.Average(), 40);
}
