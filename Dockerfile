FROM gcc:13

RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копіюємо всередину контейнера
COPY . .

# Робимо збірку та виводимо вміст папки build для перевірки
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make

# Перевіримо, де з'явився бінарник (якщо він у корені або в build)
CMD ["./build/TimerProject"]