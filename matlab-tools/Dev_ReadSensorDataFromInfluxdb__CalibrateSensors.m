%% Load InfluxDB credentials from JSON

% Get the current working directory
current_dir = pwd;

% Navigate to the project root by removing "matlab-tools"
if endsWith(current_dir, 'matlab-tools')
    project_root = fileparts(current_dir); % Go up one directory
else
    error('Current directory is not "matlab-tools". Please navigate to the correct folder.');
end

% Define the private folder path relative to the project root
private_folder = fullfile(project_root, 'private');

% Path to the credentials file
credentials_file = fullfile(private_folder, 'influxdb_credentials.json');

% Read the credentials
if isfile(credentials_file)
    fid = fopen(credentials_file, 'r');
    raw_data = fread(fid, inf, 'uint8=>char')';
    fclose(fid);
    credentials = jsondecode(raw_data);
else
    error('Credentials file not found: %s', credentials_file);
end

% Extract credentials
influxdb_url = credentials.influxdb_url;
db_name = credentials.db_name;
username = credentials.username;
password = credentials.password;

% Display confirmation for debugging
disp('Credentials loaded successfully.');



%% Query section

% Define the time range
start_time = datetime('2025-02-09 22:25:00', 'Format', 'yyyy-MM-dd''T''HH:mm:ss''Z''');
end_time = datetime('2025-02-10 08:00:00', 'Format', 'yyyy-MM-dd''T''HH:mm:ss''Z''');

% Convert datetime to string format
start_time_str = char(start_time);
end_time_str = char(end_time);

% Sensor names
sensor_names = {
    "SENSOR_3DPrinterFrontRight00_Temperature",
    "SENSOR_3DPrinterFrontRight25_Temperature",
    "SENSOR_3DPrinterFrontRight50_Temperature",
    "SENSOR_3DPrinterFrontRight75_Temperature",
    "SENSOR_3DPrinterFrontRight100_Temperature",
    "SENSOR_3DPrinterFrontTopLeft_Temperature",
    "SENSOR_3DPrinterFrontTopCentre_Temperature",
    "SENSOR_3DPrinterFrontTopRight_Temperature",
    "SENSOR_3DPrinterFrontLeft00_Temperature",
    "SENSOR_3DPrinterFrontLeft25_Temperature",
    "SENSOR_3DPrinterFrontLeft50_Temperature",
    "SENSOR_3DPrinterFrontLeft75_Temperature",
    "SENSOR_3DPrinterFrontLeft100_Temperature"
};

% Initialize storage for timestamps and temperature values
all_timestamps = {};
all_temperature_data = cell(size(sensor_names));

% Loop over each sensor and fetch data
for i = 1:length(sensor_names)
    sensor = sensor_names{i};
    
    % Construct InfluxQL query
    query = sprintf('SELECT "value" FROM "%s" WHERE time >= ''%s'' AND time <= ''%s''', ...
                    sensor, start_time_str, end_time_str);

    % Properly encode the query for URL
    query_url = urlencode(query);

    % Construct the full URL for the query
    url = sprintf("%s/query?db=%s&q=%s", influxdb_url, db_name, query_url);

    % HTTP request options with basic authentication
    options = weboptions('Username', username, 'Password', password, 'ContentType', 'json');

    % Query InfluxDB and process the response
    try
        response = webread(url, options);

        % Extract series data
        if isfield(response, 'results') && ~isempty(response.results)
            result = response.results(1); % Get the first result
            if isfield(result, 'series') && ~isempty(result.series)
                series = result.series(1); % Access the first series
                if isfield(series, 'values') && ~isempty(series.values)
                    % Extract the raw nested cell array
                    raw_values = series.values; % Cell array where each cell contains 2×1 cells

                    % Extract timestamps and values from the nested cells
                    timestamps = datetime(cellfun(@(x) x{1}, raw_values, 'UniformOutput', false), ...
                                          'InputFormat', 'yyyy-MM-dd''T''HH:mm:ss.SSS''Z''', 'TimeZone', 'UTC');
                    temperature_values = cellfun(@(x) x{2}, raw_values); % Extract numeric values

                    % Store data for this sensor
                    all_timestamps{i} = timestamps;
                    all_temperature_data{i} = temperature_values;

                    % Debugging: Display extracted data
                    fprintf('Data for sensor %s:\n', sensor);
                    disp(timestamps);
                    disp(temperature_values);
                else
                    warning('No data in "values" field for %s', sensor);
                end
            else
                warning('No "series" field in response for %s', sensor);
            end
        else
            warning('No results in response for %s', sensor);
        end
    catch ME
        warning("Failed to retrieve data for %s: %s", sensor, ME.message);
    end
end

% % Plotting section
, you will see 
% Create a figure and plot all sensor data
figure;
hold on;
for i = 1:length(sensor_names)
    if ~isempty(all_timestamps{i}) && ~isempty(all_temperature_data{i})
        plot(all_timestamps{i}, all_temperature_data{i}, '-o', 'DisplayName', sensor_names{i});
    end
end
hold off;

% Formatting
xlabel('Time');
ylabel('Temperature (°C)');
title('3D Printer Temperature Sensors');
legend('Location', 'best');
grid on;

%% Step 1: Select Reference Sensor
ref_sensor_index = 1; % Index of the reference sensor (choose one)
ref_sensor_data = all_temperature_data{ref_sensor_index};
ref_sensor_timestamps = all_timestamps{ref_sensor_index};

% Remove invalid data from the reference sensor
valid_ref_idx = isfinite(ref_sensor_timestamps) & isfinite(ref_sensor_data);
ref_sensor_timestamps = ref_sensor_timestamps(valid_ref_idx);
ref_sensor_data = ref_sensor_data(valid_ref_idx);

%% Step 2: Initialize Storage for Calibration
calibrated_data = cell(size(sensor_names));
calibration_coefficients = cell(size(sensor_names));

%% Step 3: Perform Calibration for Each Sensor
for i = 1:length(sensor_names)
    if i == ref_sensor_index
        % Skip the reference sensor (already aligned)
        calibrated_data{i} = ref_sensor_data;
        continue;
    end

    % Get current sensor data and timestamps
    sensor_data = all_temperature_data{i};
    sensor_timestamps = all_timestamps{i};

    % Remove invalid data from the current sensor
    valid_sensor_idx = isfinite(sensor_timestamps) & isfinite(sensor_data);
    sensor_timestamps = sensor_timestamps(valid_sensor_idx);
    sensor_data = sensor_data(valid_sensor_idx);

    % Interpolate the reference data to match current sensor timestamps
    ref_interp_data = interp1(ref_sensor_timestamps, ref_sensor_data, sensor_timestamps, 'linear', 'extrap');

    % Perform polynomial fit (2nd order for non-linear decay)
    p = polyfit(sensor_data, ref_interp_data, 2); % Adjust degree as needed
    calibration_coefficients{i} = p;

    % Apply the calibration
    calibrated_data{i} = polyval(p, sensor_data);
end

%% Step 4: Plot Calibrated Data
% figure;
% hold on;
% 
% % Use reference sensor timestamps for alignment
% ref_timestamps = all_timestamps{ref_sensor_index};
% 
% % Loop over all sensors to align and plot calibrated data
% for i = 1:length(sensor_names)
%     if ~isempty(all_timestamps{i}) && ~isempty(calibrated_data{i})
%         % Interpolate calibrated data onto the reference sensor timestamps
%         aligned_calibrated_data = interp1(all_timestamps{i}, calibrated_data{i}, ref_timestamps, 'linear', 'extrap');
% 
%         % Plot the aligned data
%         plot(ref_timestamps, aligned_calibrated_data, '-o', 'DisplayName', ['Calibrated ', sensor_names{i}]);
%     end
% end
% hold off;
% 
% % Formatting
% xlabel('Time');
% ylabel('Temperature (°C)');
% title('Calibrated 3D Printer Temperature Sensors');
% legend('Location', 'best');
% grid on;


% %% Step 5: Display Calibration Coefficients
% for i = 1:length(sensor_names)
%     if i ~= ref_sensor_index
%         fprintf('Calibration coefficients for %s: %s\n', sensor_names{i}, mat2str(calibration_coefficients{i}, 4));
%     end
% end

%%


% To address the issue of misaligned timestamps properly, we will first create a new sampled time grid (every 1 second within the specified time range). Then, for each sensor's data, we will find the nearest value to each sampled time. This will create a synchronized dataset across all sensors.
% 
% Steps to Resample the Data
% Create a Regular Time Grid:
% 
% Define a time grid based on your desired sampling interval (e.g., every 1 second).
% Find the Nearest Timestamp for Each Sensor:
% 
% For each sensor, find the index of the timestamp closest to each sampled time using interp1.
% Extract Data for the Synchronized Grid:
% 
% Use the indices to extract the nearest temperature value for each sampled time.
% Perform Calibration on Synchronized Data:
% 
% Now that all sensors share the same timestamps, proceed with polynomial fitting and calibration.

% % Step 1: Define Sampling Interval and Time Grid
start_time = min(cellfun(@(x) min(x), all_timestamps)); % Earliest timestamp
end_time = max(cellfun(@(x) max(x), all_timestamps));   % Latest timestamp
sample_interval = seconds(1);                          % Sampling interval: 1 second
sampled_timestamps = (start_time:sample_interval:end_time)'; % Regular time grid

% % Step 2: Resample Each Sensor's Data
resampled_data = cell(size(sensor_names)); % Resampled temperatures
for i = 1:length(sensor_names)
    % Current sensor's timestamps and data
    sensor_timestamps = all_timestamps{i};
    sensor_data = all_temperature_data{i};

    % Remove invalid data
    valid_idx = isfinite(sensor_timestamps) & isfinite(sensor_data);
    sensor_timestamps = sensor_timestamps(valid_idx);
    sensor_data = sensor_data(valid_idx);

    % Find nearest values for the sampled timestamps
    nearest_indices = interp1(sensor_timestamps, 1:length(sensor_timestamps), sampled_timestamps, 'nearest', 'extrap');
    resampled_data{i} = sensor_data(nearest_indices);
end

% % Step 3: Perform Calibration for Each Sensor
ref_sensor_index = 1; % Choose the reference sensor
ref_sensor_data = resampled_data{ref_sensor_index}; % Reference sensor's resampled data

calibrated_data = cell(size(sensor_names)); % Storage for calibrated data
calibration_coefficients = cell(size(sensor_names)); % Polynomial coefficients
for i = 1:length(sensor_names)
    if i == ref_sensor_index
        % Use reference data directly
        calibrated_data{i} = ref_sensor_data;
        continue;
    end

    % Current sensor's resampled data
    sensor_data = resampled_data{i};

    % Perform polynomial fit (2nd order)
    p = polyfit(sensor_data, ref_sensor_data, 2); % Adjust degree as needed
    calibration_coefficients{i} = p;

    % Apply the calibration
    calibrated_data{i} = polyval(p, sensor_data);
end

% % Step 4: Plot Calibrated Data
% % Step 4: Plot Calibrated Data
figure;
hold on;
for i = 1:length(sensor_names)
    plot(sampled_timestamps, calibrated_data{i}, '-o', 'DisplayName', sensor_names{i});
end
hold off;

% Formatting
xlabel('Time');
ylabel('Temperature (°C)');
title('Calibrated 3D Printer Temperature Sensors');

% Customize legend
lgd = legend('Location', 'best');
set(lgd, 'Interpreter', 'none'); % Disable LaTeX interpreter to display underscores as-is

grid on;


% % Step 5: Display Calibration Coefficients
for i = 1:length(sensor_names)
    if i ~= ref_sensor_index
        fprintf('Calibration coefficients for %s: %s\n', sensor_names{i}, mat2str(calibration_coefficients{i}, 4));
    end
end


%%

% % Step 3: Perform Calibration for Each Sensor
ref_sensor_index = 1; % Choose the reference sensor
ref_sensor_data = resampled_data{ref_sensor_index}; % Reference sensor's resampled data

calibrated_data = cell(size(sensor_names)); % Storage for calibrated data
calibration_coefficients = cell(size(sensor_names)); % Polynomial coefficients
for i = 1:length(sensor_names)
    if i == ref_sensor_index
        % Use reference data directly
        calibrated_data{i} = ref_sensor_data;
        continue;
    end

    % Current sensor's resampled data
    sensor_data = resampled_data{i};

    % Perform linear fit (1st order)
    p = polyfit(sensor_data, ref_sensor_data, 1); % Linear fit: y = mx + c
    calibration_coefficients{i} = p;

    % Apply the calibration
    calibrated_data{i} = polyval(p, sensor_data);
end

% % Step 4: Plot Calibrated Data
% % Step 4: Plot Calibrated Data
figure;
hold on;
for i = 1:length(sensor_names)
    % Convert sensor name to a character vector
    sensor_name = char(sensor_names{i});
    % Plot calibrated data
    plot(sampled_timestamps, calibrated_data{i}, '-o', 'DisplayName', ['Calibrated ', sensor_name]);
end
hold off;

% Formatting
xlabel('Time');
ylabel('Temperature (°C)');
title('Calibrated 3D Printer Temperature Sensors');

% Customize legend
lgd = legend('Location', 'best');
set(lgd, 'Interpreter', 'none'); % Ensure underscores are shown as-is
grid on;


% % Step 5: Plot Original vs. Calibrated Data
figure;
hold on;
for i = 1:length(sensor_names)
    % Convert sensor name to a character vector
    sensor_name = char(sensor_names{i});
    % Plot original data (dashed lines)
    plot(sampled_timestamps, resampled_data{i}, '--', 'DisplayName', ['Original ', sensor_name]);
    % Plot calibrated data (solid lines)
    plot(sampled_timestamps, calibrated_data{i}, '-', 'LineWidth', 1.5, ...
        'DisplayName', ['Calibrated ', sensor_name]);
end
hold off;

% Formatting for Comparison Plot
xlabel('Time');
ylabel('Temperature (°C)');
title('Original vs. Calibrated 3D Printer Temperature Sensors');
lgd = legend('Location', 'best');
set(lgd, 'Interpreter', 'none'); % Ensure underscores are displayed
grid on;

% % Step 6: Overlay Fitted Polynomials on Calibrated Data
figure;
hold on;
for i = 1:length(sensor_names)
    % Convert sensor name to a character vector
    sensor_name = char(sensor_names{i});
    
    % Plot original data (dashed lines)
    plot(sampled_timestamps, resampled_data{i}, '--', 'DisplayName', ['Original ', sensor_name]);
    % Plot calibrated data (solid lines)
    plot(sampled_timestamps, calibrated_data{i}, '-', 'LineWidth', 1.5, ...
        'DisplayName', ['Calibrated ', sensor_name]);

    % Overlay fitted polynomial (dotted lines) on the calibrated data
    p = calibration_coefficients{i}; % Get the polynomial coefficients
    fitted_values = polyval(p, calibrated_data{i}); % Evaluate the polynomial on calibrated data
    plot(sampled_timestamps, fitted_values, ':', 'LineWidth', 1.2, ...
        'DisplayName', ['Fitted ', sensor_name]);
end
hold off;

% Formatting for Fitted Polynomial Plot
xlabel('Time');
ylabel('Temperature (°C)');
title('Original, Calibrated, and Fitted Data for 3D Printer Sensors');
lgd = legend('Location', 'best');
set(lgd, 'Interpreter', 'none'); % Ensure underscores are displayed
grid on;
