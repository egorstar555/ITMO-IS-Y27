plugins {
    id("java")
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
}

tasks.test {
    useJUnitPlatform()
}

tasks.build {
    dependsOn(tasks.javadoc)
}

tasks.register<JavaExec>("runConsoleApp") {
    group = "other"
    description = "Runs the console application."
    mainClass.set("com.Starygin.Main")
    classpath = sourceSets["main"].runtimeClasspath
    standardInput = System.`in`
}