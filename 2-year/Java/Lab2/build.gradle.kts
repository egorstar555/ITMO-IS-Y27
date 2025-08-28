plugins {
    id("java")
    id("org.flywaydb.flyway") version "11.4.0"
}

group = "com.Starygin"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(platform("org.junit:junit-bom:5.9.1"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testImplementation("org.mockito:mockito-core:5.16.0")

    testImplementation("org.junit.jupiter:junit-jupiter-api:5.9.1")
    testImplementation("org.testcontainers:junit-jupiter:1.20.6")
    testImplementation("org.testcontainers:postgresql:1.20.6")
    testImplementation("ch.qos.logback:logback-classic:1.5.18")

    implementation("org.hibernate.orm:hibernate-core:6.6.11.Final")
    implementation("org.postgresql:postgresql:42.7.2")
}

buildscript {
    dependencies {
        classpath("org.postgresql:postgresql:42.7.2")
        classpath("org.flywaydb:flyway-database-postgresql:11.5.0")
    }
}

flyway {
    driver = "org.postgresql.Driver"
    url = "jdbc:postgresql://localhost:6432/postgres"
    user = "postgres"
    password = "postgres"
    schemas = arrayOf("public")
    locations = arrayOf("filesystem:src/main/resources/db/migration")
    cleanDisabled = false
}

tasks.test {
    useJUnitPlatform()
}