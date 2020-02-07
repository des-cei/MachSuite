%% MATLAB script to plot benchmark results

% Initialize environment
clear all
close all
clc

% Load data
load('workspace.mat');

%% Plot all data

% Create figure, plot and enable grid
figure(1);
bar([t_sdsoc t_artico3], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (SW)', 'SDSoC (HW)', 'ARTICo^{3} (1acc)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC SW vs ARTICo³ (1acc) vs ARTICo³ (4accs)

% Create figure, plot and enable grid
figure(2);
bar([t_sdsoc(:,1) t_artico3], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (SW)', 'ARTICo^{3} (1acc)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC HW vs ARTICo³ (1acc) vs ARTICo³ (4accs)

% Create figure, plot and enable grid
figure(3);
bar([t_sdsoc(:,2) t_artico3], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (HW)', 'ARTICo^{3} (1acc)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC SW vs ARTICo³ (1acc) data

% Create figure, plot and enable grid
figure(4);
bar([t_sdsoc(:,1) t_artico3(:,1)], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (SW)', 'ARTICo^{3} (1acc)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC SW vs ARTICo³ (4acc) data

% Create figure, plot and enable grid
figure(5);
bar([t_sdsoc(:,1) t_artico3(:,2)], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (SW)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC HW vs ARTICo³ (1acc) data

% Create figure, plot and enable grid
figure(6);
bar([t_sdsoc(:,2) t_artico3(:,1)], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (HW)', 'ARTICo^{3} (1acc)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot SDSoC HW vs ARTICo³ (4accs) data

% Create figure, plot and enable grid
figure(7);
bar([t_sdsoc(:,2) t_artico3(:,2)], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('SDSoC (HW)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);

%% Plot ARTICo³ (1acc) vs ARTICo³ (4accs) data

% Create figure, plot and enable grid
figure(8);
bar([t_artico3(:,1) t_artico3(:,2)], 1);
grid on;

% X-axis format
xticks(1:17);
set(gca,'xticklabel',l);
ax = get(gca,'XAxis');
set(ax, 'FontSize', 8);
xtickangle(45);

% Y-axis format
yticks([0.01 0.1 1 10 100 1000]);
ylabel('Execution Time (ms)');
% ax = get(gca,'YAxis');
% set(ax, 'FontSize', 10);
set(gca, 'YScale', 'log');
curtick = get(gca, 'YTick');
set(gca, 'YTickLabel', cellstr(num2str(curtick(:))))

% Legend
leg = legend('ARTICo^{3} (1acc)', 'ARTICo^{3} (4accs)', 'Location', 'Best');
set(leg, 'FontSize', 8);
set(leg,'Position',[0.385 0.8 0.25 0.1]);

% Global axes configuration
axis([0.5 17.5 0.01 1000]);
