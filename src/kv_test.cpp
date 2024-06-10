// #include <Arduino.h>
// #include <esp_task_wdt.h>
// #include <motorgo_mini.h>

// #include <atomic>

// TaskHandle_t loop_foc_task;
// TickType_t xLastWakeTime;
// void loop_foc(void* pvParameters);

// bool motors_enabled = false;
// bool enable_flag = false;
// bool disable_flag = false;

// std::atomic<float> motor_target_voltage(0.0);

// MotorGo::MotorGoMini motorgo_mini;
// MotorGo::MotorChannel& motor = motorgo_mini.ch0;

// MotorGo::ChannelConfiguration config_left;

// // Function to print at a maximum frequency
// void freq_println(String str, int freq)
// {
//   static unsigned long last_print_time = 0;
//   unsigned long now = millis();

//   if (now - last_print_time > 1000 / freq)
//   {
//     Serial.println(str);
//     last_print_time = now;
//   }
// }

// void enable_motors() { enable_flag = true; }
// void disable_motors() { disable_flag = true; }
// void setup()
// {
//   Serial.begin(115200);

//   delay(4000);

//   // Setup motor parameters

//   config_left.motor_config.pole_pairs = 11;
//   config_left.motor_config.velocity_limit = 10000;
//   config_left.motor_config.current_limit = 10000;
//   config_left.motor_config.voltage_limit = 15;
//   config_left.power_supply_voltage = 5.0;
//   config_left.motor_config.calibration_voltage = 1.0;
//   config_left.reversed = false;

//   // Setup
//   bool calibrate = true;
//   motor.init(config_left, calibrate);

//   //   Set closed-loop velocity mode
//   motor.set_control_mode(MotorGo::ControlMode::Voltage);

//   xTaskCreatePinnedToCore(
//       loop_foc,       /* Task function. */
//       "Loop FOC",     /* name of task. */
//       10000,          /* Stack size of task */
//       NULL,           /* parameter of the task */
//       1,              /* priority of the task */
//       &loop_foc_task, /* Task handle to keep track of created task */
//       1);             /* pin task to core 1 */

//   //   Enable motors
//   motor.enable();
// }

// void loop_foc(void* pvParameters)
// {
//   Serial.print("Loop FOC running on core ");
//   Serial.println(xPortGetCoreID());

//   for (;;)
//   {
//     // Service flags
//     if (enable_flag)
//     {
//       Serial.println("Motors are enabled");
//       motor.enable();
//       enable_flag = false;
//       motors_enabled = true;
//     }
//     else if (disable_flag)
//     {
//       Serial.println("Motors are disabled");
//       motor.disable();
//       disable_flag = false;
//       motors_enabled = false;
//     }

//     // Get all targets
//     float target_voltage = motor_target_voltage.load();

//     motor.set_target_voltage(target_voltage);

//     motor.loop();

//     esp_task_wdt_reset();
//   }
// }

// float get_average_velocity()
// {
//   // Let the motor stablize, wait 1 second
//   vTaskDelay(1000 / portTICK_PERIOD_MS);

//   // Collect 1000 velocity samples
//   float velocity_average;
//   for (int i = 0; i < 1000; i++)
//   {
//     velocity_average += motor.get_velocity();
//     // Wait 10 ms
//     vTaskDelay(10 / portTICK_PERIOD_MS);
//   }

//   velocity_average /= 1000;

//   return velocity_average;
// }

// void wait_for_button()
// {
//   while (digitalRead(0))
//   {
//     delay(50);
//   }

//   while (!digitalRead(0))
//   {
//     delay(50);
//   }
//   delay(50);
// }

// void loop()
// {
//   wait_for_button();
//   motor_target_voltage.store(5);

//   float velocity = get_average_velocity();
//   float rpm = velocity * 60 / (2 * PI);
//   Serial.println(String(rpm));
//   motor_target_voltage.store(0);
//   //   for (float i = 0; i < 12; i += 0.2)
//   //   {
//   //     motor_target_voltage.store(5);
//   //     delay(100);

//   //     float velocity = get_average_velocity();

//   //     // Compute RPM
//   //     float rpm = velocity * 60 / (2 * PI);

//   //     // Print RPM and voltage
//   //     Serial.println(String(rpm) + " " + String(i / sqrt(3)));
//   //   }
// }