# Здесь вы можете создавать свои исполняемые файлы (executables)
# исполянемый файл = контрольный тест

# Пример создания контрольного теста (исполняемого файла) под названием demo_benchmark.
# Исходный код предоставлен в файле demo_benchmark.cpp текущей директории.
add_executable(insert_benchmark insert_benchmark.cpp)
add_executable(find_benchmark find_benchmark.cpp)
add_executable(remove_benchmark remove_benchmark.cpp)

# Примечание: Не забываем подключить (прилинковать) библиотеку ${PROJECT_NAME} с реализацией структуры данных.
target_link_libraries(insert_benchmark PRIVATE project_paths project_warnings ${PROJECT_NAME})
target_link_libraries(find_benchmark PRIVATE project_paths project_warnings ${PROJECT_NAME})
target_link_libraries(remove_benchmark PRIVATE project_paths project_warnings ${PROJECT_NAME})