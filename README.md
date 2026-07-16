# Measure L1 Cache Latency
Run the test 20,000 times and get the average latency

`EvictCacheLineSize` about 512kb to flush out l2

Use `volatile` for `sink` variable to prevent compiler from optimizing reads and writes

There is some overhead for std::chrono timing so timing may not be fully accurate

Code is based on this [YouTube video](https://www.youtube.com/watch?v=03Os5ydbiTE)
