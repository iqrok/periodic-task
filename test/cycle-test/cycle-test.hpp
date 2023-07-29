#ifndef _CYCLE_TEST_HPP_
#define _CYCLE_TEST_HPP_

#include <TaskCycle.hpp>

#include <sys/mman.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>

using namespace std;

namespace MainRoutine {

void workload_sin(const std::vector<double>& data, double& result);
void workload_busy(void);
void workload_sleep(void);
void workload_deadline(void);

constexpr uint32_t sample_size = 300;

struct TaskConfigurations {
	struct TaskCycle::distribution_summary_s summary;
	float samples[sample_size];
	TaskCycle::task_config_t config;
	std::thread thread;
};

struct TaskConfigurations tBusy, tSleep, tDeadline;

double result_busy = 0;
std::vector<double> data_busy = {
	1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.0, 1.1,
	1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.0, 1.1, 1.2,
	1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.0, 1.1, 1.2, 1.3,
	1.4, 1.5, 1.6, 1.7, 1.8, 1.0, 1.1, 1.2, 1.3, 1.4,
	1.5, 1.6, 1.7, 1.8, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5,
	1.6, 1.7, 1.8, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6,
	1.7, 1.8, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7,
	1.8, 1.0, 1.5, 1.6, 1.7, 1.5, 1.6, 1.7, 1.8, 1.0,
	1.1, 1.2, 1.3, 1.4, 1.5, 1.7, 1.8, 2.0, 1.1, 1.2,
	1.7, 1.8, 2.0, 1.1, 1.2, 1.1, 1.2, 1.5, 1.6, 1.7,
};

double result_sleep = 0;
std::vector<double> data_sleep = {
	0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1,
	0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2,
	0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3,
	0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4,
	0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5,
	0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
	0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
	0.8, 1.0, 0.5, 0.6, 0.7, 0.6, 0.7, 0.8, 1.0, 0.1,
	0.7, 0.8, 1.0, 0.1, 0.2, 0.5, 0.6, 0.7, 0.6, 0.7,
	0.4, 0.5, 0.6, 0.7, 0.8, 0.7, 0.6, 0.7, 0.8, 1.0,
};

double result_deadline = 0;
std::vector<double> data_deadline = {
	0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1,
	0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2,
	0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3,
	0.4, 0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4,
	0.5, 0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5,
	0.6, 0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
	0.7, 0.8, 1.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7,
	0.8, 1.0, 0.5, 0.6, 0.7, 0.6, 0.7, 0.8, 1.0, 0.1,
	0.7, 0.8, 1.0, 0.1, 0.2, 0.5, 0.6, 0.7, 0.6, 0.7,
	0.4, 0.5, 0.6, 0.7, 0.8, 0.7, 0.6, 0.7, 0.8, 1.0,
};

void workload_sin(const std::vector<double>& data, double& result)
{
	double rt = 0;
	for (size_t i = 0; i < data.size(); ++i) {
		rt += data[i] * std::sin(data[i]);
	}
	result = rt;
}

void workload_busy(void)
{
	workload_sin(data_busy, result_busy);
}

void workload_sleep(void)
{
	workload_sin(data_sleep, result_sleep);
}

void workload_deadline(void)
{
	workload_sin(data_deadline, result_deadline);
}

void start()
{
	if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
		fprintf(stderr, "Failed to lock memory: %s\n", strerror(errno));
	}

	tBusy.config.affinity = 1;
	tBusy.config.step_sleep = 350;
	tBusy.config.schedule_priority = SCHED_FIFO;
	tBusy.config.nice_value = -20;
	tBusy.config.tolerance = -1;
	tBusy.config.period_ns = 750'000;
	tBusy.config.lazy_sleep = tBusy.config.period_ns - 350'000;
	tBusy.config.offset_ns = 750,
	tBusy.config.fptr = workload_busy;

	tSleep.config.affinity = 0;
	tSleep.config.step_sleep = 350;
	tSleep.config.schedule_priority = SCHED_FIFO;
	tSleep.config.priority_offset = 1;
	tSleep.config.nice_value = -20;
	tSleep.config.tolerance = -1;
	tSleep.config.period_ns = 5'000'000;
	tSleep.config.lazy_sleep = tSleep.config.period_ns - 300'000;
	tSleep.config.offset_ns = 550,
	tSleep.config.fptr = workload_sleep;

	tDeadline.config.nice_value = -20;
	tDeadline.config.period_ns = 5'000'000;
	tDeadline.config.offset_ns = 0;
	tDeadline.config.exec_time = 75'000;
	tDeadline.config.fptr = workload_deadline;

	tDeadline.thread = std::thread(&TaskCycle::routine_deadline, &tDeadline.config,
		tDeadline.samples, sample_size);

	tSleep.thread = std::thread(&TaskCycle::routine_busy, &tSleep.config,
		tSleep.samples, sample_size);

	tBusy.thread = std::thread(&TaskCycle::routine_busy, &tBusy.config,
		tBusy.samples, sample_size);
}

void stop()
{
	tBusy.config.is_running = false;
	tSleep.config.is_running = false;
	tDeadline.config.is_running = false;

	tSleep.thread.join();
	tBusy.thread.join();
	tDeadline.thread.join();

	TaskCycle::stats_summarize(&tBusy.summary, tBusy.samples, sample_size,
		tBusy.config.period_ns);

	TaskCycle::stats_summarize(&tSleep.summary, tSleep.samples, sample_size,
		tSleep.config.period_ns);

	TaskCycle::stats_summarize(&tDeadline.summary, tDeadline.samples, sample_size,
		tDeadline.config.period_ns);

	printf("\n");

	TaskCycle::stats_print("BUSY WAIT", tBusy.summary);
	TaskCycle::stats_print("SLEEP", tSleep.summary);
	TaskCycle::stats_print("DEADLINE", tDeadline.summary);

	printf("workload busy %lf\n", result_busy);
	printf("workload sleep %lf\n", result_sleep);
	printf("workload deadline %lf\n", result_deadline);
}

}

#endif
