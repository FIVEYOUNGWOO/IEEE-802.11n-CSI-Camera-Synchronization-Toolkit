% file_path = '19.dat';
% num_samples = 5;
% window_size = 10;
% newly_CSI_analyzer(file_path, num_samples, window_size)

function [] = newly_CSI_analyzer(file_path, num_samples, window_size)
    % Set font to Times New Roman
    set(0,'defaultAxesFontName', 'Times New Roman');
    set(0,'defaultTextFontName', 'Times New Roman');
    
    % Get load of saved .dat files from the toolkit
    csi_trace = read_bf_file(file_path);
    
%     disp(csi_trace{1}.timestamp_low);

    num_subcarriers = size(csi_trace{1}.csi, 3);
    num_tx = csi_trace{1}.Ntx;
    num_rx = csi_trace{1}.Nrx;

    csi_data = zeros(num_samples, num_tx, num_rx, num_subcarriers);
    
    for i = 1:num_samples
        csi_entry = csi_trace{i};
        csi_data(i, :, :, :) = csi_entry.csi;
    end
    
    raw_amplitudes = abs(csi_data);
    raw_phases = angle(csi_data);
    
    unwrapped_phases = unwrap(raw_phases, [], 4);
    
    f = (1:num_subcarriers).';
    linear_fit_phases = unwrapped_phases;
    
    for i = 1:num_samples
        for tx = 1:num_tx
            for rx = 1:num_rx
                phase_sample = squeeze(unwrapped_phases(i, tx, rx, :));
                p = polyfit(f, phase_sample, 1);
                linear_fit = polyval(p, f);
                linear_fit_phases(i, tx, rx, :) = phase_sample - linear_fit;
            end
        end
    end
    
    % Median and Uniform Filters application
    filtered_phases = unwrapped_phases; % Create a copy for filtered data
    
    for i = 1:num_samples
        for tx = 1:num_tx
            for rx = 1:num_rx
                phase_sample = squeeze(unwrapped_phases(i, tx, rx, :));

                % Apply median filter
                phase_sample = medfilt1(phase_sample);

                % Apply uniform filter (moving average filter as an example)
                phase_sample = filter(ones(1, window_size)/window_size, 1, phase_sample);
                filtered_phases(i, tx, rx, :) = phase_sample;
            end
        end
    end
    
    % Linear fitting on filtered data
    filtered_linear_fit_phases = filtered_phases; % Copy for linear fit
    
    for i = 1:num_samples
        for tx = 1:num_tx
            for rx = 1:num_rx
                phase_sample = squeeze(filtered_phases(i, tx, rx, :));
                p = polyfit(f, phase_sample, 1); % Calculate linear fitting coefficients
                linear_fit = polyval(p, f); % Calculate linear fit values for all subcarrier indices
                filtered_linear_fit_phases(i, tx, rx, :) = phase_sample - linear_fit; % Subtract linear fit from filtered unwrapped phase
            end
        end
    end
    
    figure;
    
    subplot(2,3,1);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(raw_amplitudes(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Raw CSI amplitude');
    xlabel('Subcarrier index');
    ylabel('Amplitude');
    
    subplot(2,3,2);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(raw_phases(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Raw CSI phase');
    xlabel('Subcarrier index');
    ylabel('Phase (Radians)');
    
    subplot(2,3,3);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(unwrapped_phases(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Unwrapped Phase');
    xlabel('Subcarrier index');
    ylabel('Phase (Radians)');
    
    subplot(2,3,4);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(linear_fit_phases(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Unwrapped pahse + linear fitting');
    xlabel('Subcarrier index');
    ylabel('Phase (Radians)');
    
    subplot(2,3,5);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(filtered_phases(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Unwrapped phase + filters');
    xlabel('Subcarrier index');
    ylabel('Phase (Radians)');
    
    subplot(2,3,6);
    hold on;
    for i = 1:num_samples
        plot(1:num_subcarriers, squeeze(mean(mean(filtered_linear_fit_phases(i, :, :, :), 2), 3)), 'LineWidth', 1.5);
    end
    hold off;
    title('Unwrapped phase + filters + linear fitting');
    xlabel('Subcarrier index');
    ylabel('Phase (Radians)');

    set(gcf, 'Position', [100, 100, 1500, 720]); % Resize figure window if needed
end