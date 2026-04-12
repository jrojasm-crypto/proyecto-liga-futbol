Nombre del proyecto: Sistema de Liga de Fútbol  
Estudiante: Julian Felipe Rojas Molina  
Grupo: G02
Materia: Pensamiento Algoritmico
Lenguaje: C++  
Tipo: Aplicación de consola

## Descripción: Programa que gestiona una liga de fútbol completa. Lee configuración desde archivo externo, registra y persiste resultados de partidos, calcula tabla de posiciones actualizada, y mantiene historial de jornadas. Todo el estado persiste al cerrar el programa.

## Instrucciones de compilación:

\`\`\`bash
g++ -o liga src/main.cpp
\`\`\`

## Instrucciones de ejecución:

\`\`\`bash
./liga
\`\`\`

## Formato de config.txt:

nombre=Premier League 2025-2026
puntos_victoria=3
puntos_empate=1
puntos_derrota=0
equipo=Arsenal
equipo=Aston Villa
equipo=Bournemouth
equipo=Brentford
equipo=Brighton
equipo=Burnley
equipo=Chelsea
equipo=Crystal Palace
equipo=Everton
equipo=Fulham
equipo=Leeds United
equipo=Liverpool
equipo=Manchester City
equipo=Manchester United
equipo=Newcastle United
equipo=Nottingham Forest
equipo=Sunderland
equipo=Tottenham
equipo=West Ham United
equipo=Wolverhampton

## Decisiones de diseño importantes:

- Modularización: Cada función tiene responsabilidad única y clara
- Punteros: Usados en \`actualiza_equipo()\` para modificar structs directamente
- Arrays: Se usan arrays estáticos en lugar de vectores para simplicidad
- Structs: Diseño simple y principiante sin complejidad innecesaria
- Archivos: Persistencia en texto plano sin bases de datos
- Tabla: Desempate por puntos → diferencia goles → goles a favor
- Lectura manual: Se parsean strings sin usar getline ni stoi
- Validación: Detección de partidos duplicados en misma jornada

## Estructura del repositorio

\`\`\`
liga-futbol/
├── src/
│   └── main.cpp
├── data/
│   ├── config.txt
│   ├── partidos.txt
│   └── fechas.txt
├── README.md
└── .gitignore
\`\`\`


Última actualización: 11/04/2026
