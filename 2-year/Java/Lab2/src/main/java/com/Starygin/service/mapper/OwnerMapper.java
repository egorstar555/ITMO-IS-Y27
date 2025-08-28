package com.Starygin.service.mapper;

import com.Starygin.data.entity.Owner;
import com.Starygin.data.entity.Pet;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.dto.PetSimpleDto;

import java.util.ArrayList;
import java.util.List;

public class OwnerMapper implements Mapper<Owner, OwnerDto> {

    @Override
    public Owner toEntity(OwnerDto dto) {
        Owner owner = new Owner();
        owner.setId(dto.id());
        owner.setName(dto.name());
        owner.setBirthday(dto.birthday());

        List<Pet> pets = new ArrayList<>();

        for (PetSimpleDto petDto : dto.pets()) {
            Pet pet = new Pet(petDto.name(), petDto.birthday(), petDto.color(), owner);
            pet.setId(petDto.id());

            pets.add(pet);
        }

        owner.setPets(pets);

        return owner;
    }

    @Override
    public OwnerDto toDto(Owner entity) {
        List<PetSimpleDto> pets = new ArrayList<>();

        for (Pet pet : entity.getPets()) {
            pets.add(new PetSimpleDto(pet.getId(), pet.getName(), pet.getBirthday(), pet.getColor()));
        }

        return new OwnerDto(entity.getId(), entity.getName(), entity.getBirthday(), pets);
    }
}
