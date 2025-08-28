import com.Starygin.Main;

import static io.restassured.RestAssured.given;
import static org.hamcrest.Matchers.*;

import com.Starygin.model.entity.*;
import com.Starygin.model.repository.OwnerRepository;
import com.Starygin.model.repository.UserRepository;
import com.Starygin.service.security.SecurityConfig;
import io.restassured.RestAssured;
import io.restassured.http.ContentType;

import java.util.List;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.server.LocalServerPort;
import org.springframework.test.context.DynamicPropertyRegistry;
import org.springframework.test.context.DynamicPropertySource;
import org.testcontainers.containers.PostgreSQLContainer;
import com.Starygin.model.repository.PetRepository;

import java.time.LocalDate;

@SpringBootTest(classes = Main.class, webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
class Lab4Tests {
    @LocalServerPort
    private Integer port;

    private static final PostgreSQLContainer<?> postgres = new PostgreSQLContainer<>("postgres:16-alpine")
            .withDatabaseName("testdb")
            .withUsername("testuser")
            .withPassword("testpass");

    @BeforeAll
    static void beforeAll() {
        postgres.start();
    }

    @AfterAll
    static void afterAll() {
        postgres.stop();
    }

    @DynamicPropertySource
    static void overrideProperties(DynamicPropertyRegistry registry) {
        registry.add("spring.datasource.url", postgres::getJdbcUrl);
        registry.add("spring.datasource.username", postgres::getUsername);
        registry.add("spring.datasource.password", postgres::getPassword);
        registry.add("spring.jpa.hibernate.ddl-auto", () -> "create-drop");
        registry.add("spring.jpa.generate-ddl", () -> "true");
        registry.add("spring.jpa.show-sql", () -> "true");
        registry.add("spring.jpa.properties.hibernate.format_sql", () -> "true");
    }

    @Autowired
    private PetRepository petRepository;

    @Autowired
    private OwnerRepository ownerRepository;

    @Autowired
    private UserRepository userRepository;

    @BeforeEach
    void setUp() {
        RestAssured.baseURI = "http://localhost:" + port;
        petRepository.deleteAll();
        ownerRepository.deleteAll();
        userRepository.deleteAll();
    }

    @Test
    void shouldGetAllPets() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);

        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);

        List<Pet> pets = List.of(
                new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner),
                new Pet("jerry", LocalDate.of(2010, 2, 2), PET_COLOR.green, 15.0, owner)
        );

        owner.addPet(pets.get(0));
        owner.addPet(pets.get(1));

        ownerRepository.save(owner);
        petRepository.saveAll(pets);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/pets")
                .then()
                .statusCode(200)
                .body(".", hasSize(2));
    }

    @Test
    void shouldGetAllOwners() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);

        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);

        List<Pet> pets = List.of(
                new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner),
                new Pet("jerry", LocalDate.of(2010, 2, 2), PET_COLOR.green, 15.0, owner)
        );

        owner.addPet(pets.get(0));
        owner.addPet(pets.get(1));

        ownerRepository.save(owner);
        petRepository.saveAll(pets);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/owners")
                .then()
                .statusCode(200)
                .body(".", hasSize(1));
    }


    @Test
    void shouldGetPetById() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);

        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);

        ownerRepository.save(owner);

        Pet pet = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        pet = petRepository.save(pet);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/pets/" + pet.getId())
                .then()
                .statusCode(200)
                .body("name", equalTo("tom"))
                .body("color", equalTo("orange"))
                .body("tailLength", equalTo(14.0f));
    }

    @Test
    void shouldReturnNotFoundWhenPetNotExists() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        userRepository.save(user);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/pets/999")
                .then()
                .statusCode(404);
    }

    @Test
    void shouldFindPetsByName() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);

        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);

        ownerRepository.save(owner);

        Pet pet1 = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        Pet pet2 = new Pet("jerry", LocalDate.of(2010, 2, 2), PET_COLOR.green, 15.0, owner);
        petRepository.saveAll(List.of(pet1, pet2));

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/pets/name/tom")
                .then()
                .statusCode(200)
                .body(".", hasSize(1))
                .body("[0].name", equalTo("tom"));
    }

    @Test
    void shouldCreatePet() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);
        owner = ownerRepository.save(owner);

        String petJson = """
                {
                      "name": "tom",
                      "birthday": "2025-03-03",
                      "color": "pink",
                      "tailLength": 10.5,
                      "owner": {
                        "id": %d,
                        "name": "bob",
                        "birthday": "2020-01-01"
                      },
                      "friends": []
                
                }
                """.formatted(owner.getId());

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .body(petJson)
                .when()
                .post("/api/pets")
                .then()
                .statusCode(201)
                .body("name", equalTo("tom"))
                .body("color", equalTo("pink"))
                .body("tailLength", equalTo(10.5f));
    }

    @Test
    void shouldUpdatePet() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);
        owner = ownerRepository.save(owner);

        Pet pet = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        pet = petRepository.save(pet);

        String updatedPetJson = """
                {
                    "id": %d,
                    "name": "updated",
                    "birthday": "2000-01-01",
                    "color": "violet",
                    "tailLength": 20.0,
                      "owner": {
                        "id": %d,
                        "name": "bob",
                        "birthday": "2020-01-01"
                      },
                      "friends": []
                }
                """.formatted(pet.getId(), owner.getId());

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .body(updatedPetJson)
                .when()
                .put("/api/pets/" + pet.getId())
                .then()
                .statusCode(200)
                .body("name", equalTo("updated"))
                .body("color", equalTo("violet"))
                .body("tailLength", equalTo(20.0F));
    }

    @Test
    void shouldDeletePet() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);
        owner = ownerRepository.save(owner);

        Pet pet = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        pet = petRepository.save(pet);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .delete("/api/pets/" + pet.getId())
                .then()
                .statusCode(200);

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .when()
                .get("/api/pets/" + pet.getId())
                .then()
                .statusCode(404);
    }

    @Test
    void shouldReturnBadRequestWhenIdsMismatch() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);
        owner = ownerRepository.save(owner);

        Pet pet = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        pet = petRepository.save(pet);

        String updatedPetJson = """
                {
                    "id": 999,
                    "name": "updated",
                    "birthday": "2000-01-01",
                    "color": "black",
                    "weight": 20.0,
                    "ownerId": %d
                }
                """.formatted(owner.getId());

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .body(updatedPetJson)
                .when()
                .put("/api/pets/" + pet.getId())
                .then()
                .statusCode(400);
    }

    @Test
    void shouldFindPetsWithPaginationAndFilter() {
        User user = new User("admin", SecurityConfig.passwordEncoder().encode("admin"), USER_ROLE.admin);
        Owner owner = new Owner("bob", LocalDate.of(2020, 1, 1));
        owner.setUser(user);
        owner = ownerRepository.save(owner);

        Pet pet1 = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.orange, 14.0, owner);
        Pet pet2 = new Pet("jerry", LocalDate.of(2010, 2, 2), PET_COLOR.green, 15.0, owner);
        Pet pet3 = new Pet("spike", LocalDate.of(2015, 3, 3), PET_COLOR.orange, 20.0, owner);
        petRepository.saveAll(List.of(pet1, pet2, pet3));

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .param("page", 0)
                .param("size", 2)
                .when()
                .get("/api/pets/page-and-filter")
                .then()
                .statusCode(200)
                .body(".", hasSize(2));

        given()
                .auth().basic("admin", "admin")
                .contentType(ContentType.JSON)
                .param("color", "orange")
                .when()
                .get("/api/pets/page-and-filter")
                .then()
                .statusCode(200)
                .body(".", hasSize(2))
                .body("color", everyItem(equalTo("orange")));
    }
}