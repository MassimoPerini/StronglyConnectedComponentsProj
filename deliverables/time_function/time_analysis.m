clear;
clc;
close all;    

% V, E, #sccs, boost, bV, tarjan, tV, nuutila1, n1V, nuutila1_it, ...
csv_file = "./report1000.csv";

reportSet = importfile(csv_file, 2, Inf);

reportSet = reportSet{:, [1, 2, 3, 4, 6, 8, 10, 12, 14, 16, 18, 20]};
% V, E, #sccs, boost, tarjan, nuutila1, nuutila1_it, ...

inputs = reportSet(:, [1, 2]);

%% Tarjan linear regression
target = reportSet(:, 5);
linearModel = fitlm(inputs, target)

%% Tarjan Graph
figure();
title("Tarjan Time Analysis");
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 5));
xlabel("#V");
ylabel("#E");
zlabel("time (ms)");
hold off;

%% Iterative Algorithms
figure();
title("Iterative Algorithms Time Comparison");

scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 7));
hold on;
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 9));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 12), 'x');

legend("nuutila_1^{iter}", "nuutila_2^{iter}", "pearce_2^{iter}");
xlabel("#V");
ylabel("#E");
zlabel("time (ms)");
hold off;

%% Recursive Algorithms
figure();
title("Recursive Algorithms Time Comparison");
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 5));
hold on;

% draw other graph in same figure for comparisons
%...
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 6));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 8), 'x');
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 10));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 11), 'x');
legend("tarjan", "nuutila_1", "nuutila_2", "pearce_1", "pearce_2");
xlabel("#V");
ylabel("#E");
zlabel("time (ms)");
hold off;

%% All algorithms
figure();
title("All Algorithm Time Comparison");
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 5), 's');
hold on;

% draw other graph in same figure for comparisons
%...
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 6));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 7));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 8), 'x');
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 9), 'x');
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 10));
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 11), 'x');
scatter3(reportSet(:,1), reportSet(:,2), reportSet(:, 12));
legend("tarjan", "nuutila_1", "nuutila_1^{iter}", "nuutila_2", "nuutila_2^{iter}", "pearce_1", "pearce_2", "pearce_2^{iter}");
xlabel("#V");
ylabel("#E");
zlabel("time (ms)");
hold off;
