package com.Starygin;

import com.Starygin.data.entity.Owner;
import com.Starygin.data.entity.PET_COLOR;
import com.Starygin.data.entity.Pet;
import com.Starygin.data.repository.OwnerRepository;
import com.Starygin.data.repository.PetRepository;
import com.Starygin.service.PetService;
import com.Starygin.service.mapper.PetMapper;
import jakarta.persistence.EntityManager;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

import java.time.LocalDate;

public class Main {
    public static void main(String[] args) {
        Owner owner = new Owner(
                "bob",
                LocalDate.of(1900, 1, 1)
        );

        Pet tom = new Pet(
                "tom",
                LocalDate.of(2000, 1, 1),
                PET_COLOR.green,
                owner
        );

        Pet jerry = new Pet(
                "jerry",
                LocalDate.of(2000, 1, 1),
                PET_COLOR.orange,
                owner
        );

        owner.addPet(tom);
        owner.addPet(jerry);

        tom.addFriend(jerry);
        jerry.addFriend(tom);

        Configuration configuration = new Configuration();
        configuration.configure("hibernate.cfg.xml");

        try (
                SessionFactory sessionFactory = configuration.buildSessionFactory();
                EntityManager entityManager = sessionFactory.createEntityManager()
        ) {
            entityManager.getTransaction().begin();

            OwnerRepository ownerRepository = new OwnerRepository(entityManager);
            PetRepository petRepository = new PetRepository(entityManager);

            PetService petService = new PetService(petRepository, new PetMapper());

            System.out.println(petService.findAll());

//            ownerRepository.save(owner);
            entityManager.getTransaction().commit();
        }
    }
}