#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <string>

class Elevator
{
 private:
    int current_floor;
    int total_floors;
    std::queue<int> requests;
    bool running;

 public:
    explicit Elevator(int total_floors) : total_floors(total_floors), current_floor(1), running(true)
    {
    }

    void call(int floor)
    {
        requests.push(floor);
    }

    void stop()
    {
        running = false;
    }

    void run()
    {
        while (running)
        {
            if (!requests.empty())
            {
                int target_floor = requests.front();
                requests.pop();

                while (current_floor != target_floor)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    if (current_floor < target_floor)
                    {
                        current_floor++;
                        std::cout << "Лифт движется вверх, текущий этаж: " << current_floor << std::endl;
                    }
                    else
                    {
                        current_floor--;
                        std::cout << "Лифт движется вниз, текущий этаж: " << current_floor << std::endl;
                    }
                }

                std::cout << "Лифт остановился на этаже: " << current_floor << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
};

int main()
{
    int total_floors;
    std::cout << "Введите количество этажей в здании: ";
    std::cin >> total_floors;

    Elevator elevator(total_floors);

    std::thread elevator_thread([&]()
    { elevator.run(); });

    while (true)
    {
        std::string input;
        std::cout << "Введите этаж для вызова лифта (1-" << total_floors << ") или 'exit' для выхода: ";
        std::cin >> input;

        if (input == "exit")
        {
            std::cout << "Вы вышли из лифта" << std::endl;
            elevator.stop();
            elevator_thread.join();
            return 0;
        }
        else
        {
            try
            {
                int floor = std::stoi(input);
                if (floor >= 1 && floor <= total_floors)
                {
                    elevator.call(floor);
                }
                else
                {
                    std::cout << "Введен некорректный этаж" << std::endl;
                }
            }
            catch (std::invalid_argument&)
            {
                std::cout << "Введено некорректное значение" << std::endl;
            }
        }
    }
}
