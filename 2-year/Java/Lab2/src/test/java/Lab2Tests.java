import com.Starygin.data.entity.Owner;
import com.Starygin.data.entity.PET_COLOR;
import com.Starygin.data.entity.Pet;
import com.Starygin.data.repository.OwnerRepository;
import com.Starygin.data.repository.PetRepository;
import com.Starygin.service.OwnerService;
import com.Starygin.service.PetService;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.dto.PetDto;
import com.Starygin.service.mapper.OwnerMapper;
import com.Starygin.service.mapper.PetMapper;
import org.hibernate.boot.Metadata;
import org.hibernate.boot.MetadataSources;
import org.hibernate.boot.registry.StandardServiceRegistry;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.testcontainers.containers.PostgreSQLContainer;
import org.testcontainers.junit.jupiter.Container;
import org.testcontainers.junit.jupiter.Testcontainers;

import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;

import java.time.LocalDate;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

@Testcontainers
class Lab2Tests {

    @Container
    private static final PostgreSQLContainer<?> postgres = new PostgreSQLContainer<>("postgres:16-alpine")
            .withDatabaseName("testdb")
            .withUsername("testuser")
            .withPassword("testpass");

    private static EntityManagerFactory emf;
    private static EntityManager em;
    private static PetService petService;
    private static OwnerService ownerService;

    @BeforeAll
    static void beforeAll() {
        postgres.start();
    }

    @AfterAll
    static void afterAll() {
        postgres.stop();
    }

    @BeforeAll
    static void setUp() {
        StandardServiceRegistry registry = new StandardServiceRegistryBuilder()
                .applySetting("hibernate.connection.url", postgres.getJdbcUrl())
                .applySetting("hibernate.connection.username", postgres.getUsername())
                .applySetting("hibernate.connection.password", postgres.getPassword())
                .applySetting("hibernate.dialect", "org.hibernate.dialect.PostgreSQLDialect")
                .applySetting("hibernate.hbm2ddl.auto", "create-drop")
                .build();

        MetadataSources sources = new MetadataSources(registry)
                .addAnnotatedClass(Pet.class)
                .addAnnotatedClass(Owner.class);

        Metadata metadata = sources.getMetadataBuilder().build();

        emf = metadata.getSessionFactoryBuilder().build();
        em = emf.createEntityManager();

        PetRepository petRepository = new PetRepository(em);
        OwnerRepository ownerRepository = new OwnerRepository(em);

        PetMapper petMapper = new PetMapper();
        OwnerMapper ownerMapper = new OwnerMapper();

        petService = new PetService(petRepository, petMapper);
        ownerService = new OwnerService(ownerRepository, ownerMapper);
    }

    @AfterAll
    static void tearDown() {
        em.close();
        emf.close();
    }

    @BeforeEach
    void clearDatabase() {
        em.getTransaction().begin();

        em.createNativeQuery("DELETE FROM pet_friends").executeUpdate();
        em.createNativeQuery("DELETE FROM pets").executeUpdate();
        em.createNativeQuery("DELETE FROM owners").executeUpdate();

        em.getTransaction().commit();
    }

    @Test
    void findAll_shouldReturnEmptyList_whenNoPets() {
        // Act
        List<PetDto> result = petService.findAll();

        // Assert
        assertTrue(result.isEmpty());
    }

    @Test
    void findAll_shouldReturnAllPets() {
        // Arrange
        Owner owner = new Owner("Bob", LocalDate.of(1900, 1, 1));
        Pet pet1 = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.green, owner);
        Pet pet2 = new Pet("jerry", LocalDate.of(2000, 1, 1), PET_COLOR.orange, owner);

        em.getTransaction().begin();
        em.persist(owner);
        em.persist(pet1);
        em.persist(pet2);
        em.getTransaction().commit();

        // Act
        List<PetDto> result = petService.findAll();

        // Assert
        assertEquals(2, result.size());
        assertPetDtoEquals(pet1, result.get(0));
        assertPetDtoEquals(pet2, result.get(1));
    }

    @Test
    void findAll_shouldReturnPetsWithFriends() {
        // Arrange
        Owner owner = new Owner("Bob", LocalDate.of(1900, 1, 1));
        Pet pet1 = new Pet("tom", LocalDate.of(2000, 1, 1), PET_COLOR.green, owner);
        Pet pet2 = new Pet("jerry", LocalDate.of(2000, 1, 1), PET_COLOR.orange, owner);
        pet1.addFriend(pet2);

        em.getTransaction().begin();

        em.persist(owner);
        em.persist(pet1);
        em.persist(pet2);

        em.getTransaction().commit();

        // Act
        List<PetDto> result = petService.findAll();

        // Assert
        assertEquals(2, result.size());

        PetDto pet1Dto = result.stream()
                .filter(p -> p.name().equals("tom"))
                .findFirst()
                .orElseThrow();
        assertEquals(1, pet1Dto.friends().size());
        assertEquals("jerry", pet1Dto.friends().getFirst().name());
    }

    private void assertPetDtoEquals(Pet expected, PetDto actual) {
        assertEquals(expected.getId(), actual.id());
        assertEquals(expected.getName(), actual.name());
        assertEquals(expected.getBirthday(), actual.birthday());
        assertEquals(expected.getColor(), actual.color());
        assertEquals(expected.getOwner().getId(), actual.owner().id());
        assertEquals(expected.getOwner().getName(), actual.owner().name());
        assertEquals(expected.getOwner().getBirthday(), actual.owner().birthday());
    }

    @Test
    void findAll_shouldReturnEmptyList_whenNoOwners() {
        // Act
        List<OwnerDto> result = ownerService.findAll();

        // Assert
        assertTrue(result.isEmpty());
    }

    @Test
    void findAll_shouldReturnTwoOwners() {
        // Arrange
        Owner owner1 = new Owner("Bob", LocalDate.of(1900, 1, 1));
        Owner owner2 = new Owner("Mary", LocalDate.of(1922, 2, 2));


        em.getTransaction().begin();

        em.persist(owner1);
        em.persist(owner2);

        em.getTransaction().commit();

        // Act
        List<OwnerDto> result = ownerService.findAll();

        // Assert
        assertEquals(2, result.size());
    }
}