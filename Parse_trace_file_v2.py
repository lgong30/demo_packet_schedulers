import sys
import time
import datetime
# import numpy as np

start_time = time.time()

#------------------------------------------------------
# Constants
# TIMEOUT = float("Inf")
TIMEOUT = 1800

filepath_trace=sys.argv[1] # Filename of trace file

filename_trace = filepath_trace.split("/")[-1]
filepath_flow_stats = "../Trace/Trace_Statistics_v2/Flow_stats."+"TO_{:s}.".format(str(TIMEOUT))+filename_trace
filepath_flow_stats_noID = "../Trace/Trace_Statistics_v2/Flow_stats_noID."+"TO_{:s}.".format(str(TIMEOUT))+filename_trace
filepath_flow_detail = "../Trace/Trace_Statistics_v2/Flow_detail."+"TO_{:s}.".format(str(TIMEOUT))+filename_trace
filepath_overall_stats = "../Trace/Trace_Statistics_v2/Overall_stats."+"TO_{:s}.".format(str(TIMEOUT))+filename_trace

# Scan sorted trace file and write per-flow stats
trace_start = float("Inf")
trace_end = - float("Inf")
list_flow_timestamp = []
prev_flow_id = ""
total_packets = 0
total_flows = 0
avg_flow_pk_rate = 0
total_nontrivial_flow = 0
line_count = 0
f_flow_stats = open(filepath_flow_stats, "w+")
f_flow_stats_noID = open(filepath_flow_stats_noID, "w+")
f_flow_detail = open(filepath_flow_detail, "w+")
with open(filepath_trace, "r") as f_trace:
	for line in f_trace:
		line_count += 1
		if line_count % 100000 == 0:
			print "Processed line ({}): {:d}".\
				format(datetime.timedelta(seconds = time.time() - start_time), 
				line_count)
		if line[0] == "!":
			continue
		line = line.strip("\n")
		sep_line = line.split()

		ip_proto, ip_src, ip_dst, sport, dport, timestamp = sep_line
		timestamp = float(timestamp)

		trace_start = min(trace_start, timestamp)
		trace_end = max(trace_end, timestamp)
		total_packets += 1

		flow_id = '|'.join([ip_proto, ip_src, ip_dst, sport, dport])
		if line_count > 1:
			if ((flow_id != prev_flow_id and len(list_flow_timestamp) > 0)
				or (timestamp - list_flow_timestamp[-1] > TIMEOUT)):
				total_flows += 1
				flow_size = len(list_flow_timestamp)
				flow_start = list_flow_timestamp[0]
				flow_end = list_flow_timestamp[-1]
				flow_duration = flow_end - flow_start
				if flow_duration == 0:
					flow_pk_rate = 0
				else:
					flow_pk_rate = (flow_size - 1) / flow_duration
					avg_flow_pk_rate += flow_pk_rate
					total_nontrivial_flow += 1
				print>>f_flow_stats, "{:s},{:.6f},{:d},{:.6f},{:.6f}".\
					format(prev_flow_id, flow_start, 
					flow_size, flow_duration, 
					flow_pk_rate)
				print>>f_flow_stats_noID, "{:.6f},{:d},{:.6f},{:.6f}".\
					format(flow_start, 
					flow_size, flow_duration, 
					flow_pk_rate)
				print>>f_flow_detail, prev_flow_id + ',' \
					+ ','.join(format(x, ".6f") for x in list_flow_timestamp)
				list_flow_timestamp = []
		prev_flow_id = flow_id
		list_flow_timestamp.append(timestamp)
# Deal with the last flow
total_flows += 1
flow_size = len(list_flow_timestamp)
flow_start = list_flow_timestamp[0]
flow_end = list_flow_timestamp[-1]
flow_duration = flow_end - flow_start
if flow_duration == 0:
	flow_pk_rate = 0
else:
	flow_pk_rate = (flow_size - 1) / flow_duration
	avg_flow_pk_rate += flow_pk_rate
	total_nontrivial_flow += 1
print>>f_flow_stats, "{:s},{:.6f},{:d},{:.6f},{:.6f}".\
	format(prev_flow_id, flow_start, 
	flow_size, flow_duration, 
	flow_pk_rate)
print>>f_flow_stats_noID, "{:.6f},{:d},{:.6f},{:.6f}".\
	format(flow_start, 
	flow_size, flow_duration, 
	flow_pk_rate)
print>>f_flow_detail, prev_flow_id + ',' \
	+ ','.join(format(x, ".6f") for x in list_flow_timestamp)
# Close files
f_flow_stats.close()
f_flow_stats_noID.close()
f_flow_detail.close()

# Calculate stats
trace_duration = trace_end - trace_start
packet_rate = (total_packets - 1) / trace_duration
flow_rate = (total_flows - 1) / trace_duration
avg_flow_pk_rate /= total_nontrivial_flow

# Print overall stats
f_overall_stats = open(filepath_overall_stats, "w+")
print>>f_overall_stats, "Trace start and end time = ({:f}, {:f})".format(trace_start, trace_end)
print>>f_overall_stats, "Trace duration = {:f}".format(trace_duration)
print>>f_overall_stats, "Number of packets = {:d}".format(total_packets)
print>>f_overall_stats, "Number of flows = {:d}".format(total_flows)
print>>f_overall_stats, "Average flow size = {:f}".format(total_packets/total_flows)
print>>f_overall_stats, "Average flow rate = {:f}".format(avg_flow_pk_rate)
print>>f_overall_stats, "Overall packet rate = {:f}".format(packet_rate)
print>>f_overall_stats, "Overall flow rate = {:f}".format(flow_rate)
f_overall_stats.close()

print "--Running time:", datetime.timedelta(seconds = time.time() - start_time)